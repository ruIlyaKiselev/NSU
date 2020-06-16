package Tetris.View;

import Tetris.Controller.CsvWriter;
import Tetris.Controller.Window;
import Tetris.Controller.ImageLoader;
import Tetris.Model.Shape;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;
import java.io.FileNotFoundException;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JPanel;
import javax.swing.Timer;


public class Board extends JPanel implements KeyListener, MouseListener, MouseMotionListener
{
    // images
    private BufferedImage blocks;
    private BufferedImage pause;
    private BufferedImage refresh;
    private BufferedImage home;
    private BufferedImage[] homeButton = new BufferedImage[2];

    //board dimensions (the playing area)
    private final static int boardHeight = 20;
    private final static int boardWidth = 10;

    // block size (one cell)
    private final int blockSize = 30;

    // play area (array 10x20)
    private int[][] board = new int[boardHeight][boardWidth];

    // array with all the possible shapes (I, T, ...)
    private Tetris.Model.Shape[] shapes = new Tetris.Model.Shape[7];

    // shapes for playing
    private static Tetris.Model.Shape currentShape;
    private static Tetris.Model.Shape nextShape;

    // game loop
    private Timer looper;
    private int FPS = 60;
    private int delay = 1000 / FPS;
    boolean recordSaveAllow = true;

    // mouse events variables
    private int mouseX;
    private int mouseY;
    private boolean leftClick = false;
    private Rectangle stopBounds;
    private Rectangle refreshBounds;
    private Rectangle homeBounds;
    private boolean gamePaused = false;
    private boolean gameOver = false;

    private String playerName;

    private Tetris.Controller.Window window;

    // buttons press lapse
    private Timer buttonLapse = new Timer(1000, new ActionListener() {

        @Override
        public void actionPerformed(ActionEvent e) {
            buttonLapse.stop();
        }});

    // score
    private int score = 0;

    public Board(Tetris.Controller.Window window) {

        this.window = window;
        setBackground(Color.darkGray);

        // load Assets
        blocks = ImageLoader.loadImage("/tiles.png");
        pause = ImageLoader.loadImage("/pause.png");
        refresh = ImageLoader.loadImage("/refresh.png");
        try {
            home = ImageIO.read(Board.class.getResource("/home.png"));
        } catch (IOException e) {
            e.printStackTrace();
        }
        homeButton[0] = home.getSubimage(0, 0, 100, 80);
        homeButton[1] = home.getSubimage(100, 0, 100, 80);

        mouseX = 0;
        mouseY = 0;

        homeBounds = new Rectangle(330, 400 + home.getHeight() + 20, home.getWidth() / 2,
                home.getHeight() + home.getHeight() / 2);
        stopBounds = new Rectangle(350, 400 + 10, pause.getWidth(), pause.getHeight() + pause.getHeight() / 2);
        refreshBounds = new Rectangle(350, 400 - refresh.getHeight() - 10, refresh.getWidth(),
                refresh.getHeight() + refresh.getHeight() / 2);

        // create game looper
        looper = new Timer(delay, new GameLooper());

        // create shapes
        shapes[0] = new Tetris.Model.Shape(new int[][] {
                {1, 1, 1, 1}   // I shape;
        }, blocks.getSubimage(0, 0, blockSize, blockSize), this, 1);

        shapes[1] = new Tetris.Model.Shape(new int[][]{
                {1, 1, 1},
                {0, 1, 0},   // T shape;
        }, blocks.getSubimage(blockSize, 0, blockSize, blockSize), this, 2);

        shapes[2] = new Tetris.Model.Shape(new int[][]{
                {1, 1, 1},
                {1, 0, 0},   // L shape;
        }, blocks.getSubimage(blockSize*2, 0, blockSize, blockSize), this, 3);

        shapes[3] = new Tetris.Model.Shape(new int[][]{
                {1, 1, 1},
                {0, 0, 1},   // J shape;
        }, blocks.getSubimage(blockSize*3, 0, blockSize, blockSize), this, 4);

        shapes[4] = new Tetris.Model.Shape(new int[][]{
                {0, 1, 1},
                {1, 1, 0},   // S shape;
        }, blocks.getSubimage(blockSize*4, 0, blockSize, blockSize), this, 5);

        shapes[5] = new Tetris.Model.Shape(new int[][]{
                {1, 1, 0},
                {0, 1, 1},   // Z shape;
        }, blocks.getSubimage(blockSize*5, 0, blockSize, blockSize), this, 6);

        shapes[6] = new Tetris.Model.Shape(new int[][]{
                {1, 1},
                {1, 1},   // O shape;
        }, blocks.getSubimage(blockSize*6, 0, blockSize, blockSize), this, 7);
    }

    private void update() {
        if (stopBounds.contains(mouseX, mouseY) && leftClick && !buttonLapse.isRunning() && !gameOver)
        {
            buttonLapse.start();
            gamePaused = !gamePaused;
        }

        if (refreshBounds.contains(mouseX, mouseY) && leftClick && !buttonLapse.isRunning()) {
            recordSaveAllow = true;
            buttonLapse.start();
            startGame();
        }

        if (homeBounds.contains(mouseX, mouseY) && leftClick && !buttonLapse.isRunning()) {
            buttonLapse.start();
            window.showMenu();
        }

        if(gamePaused || gameOver)
        {
            return;
        }
        currentShape.update();
    }

    public void paintComponent(Graphics g) {

        super.paintComponent(g);
        Graphics2D g2d = (Graphics2D)g;

        for (int row = 0; row < board.length; row++)  {
            for (int col = 0; col < board[row].length; col ++) {
                if (board[row][col] != 0) {
                    g2d.drawImage(blocks.getSubimage((board[row][col] - 1)*blockSize,
                            0, blockSize, blockSize), col*blockSize, row*blockSize, null);
                }
            }
        }

        for (int row = 0; row < nextShape.getCoords().length; row ++) {
            for (int col = 0; col < nextShape.getCoords()[0].length; col ++) {
                if (nextShape.getCoords()[row][col] != 0) {
                    g2d.drawImage(nextShape.getBlock(), col*30 + 320, row*30 + 50, null);
                }
            }
        }
        currentShape.render(g2d);

        if (stopBounds.contains(mouseX, mouseY)) {
            g2d.drawImage(pause.getScaledInstance(pause.getWidth() + 3, pause.getHeight() + 3,
                    BufferedImage.SCALE_DEFAULT), stopBounds.x + 3, stopBounds.y + 3, null);
        } else {
            g2d.drawImage(pause, stopBounds.x, stopBounds.y, null);
        }

        if (refreshBounds.contains(mouseX, mouseY)) {
            g2d.drawImage(refresh.getScaledInstance(refresh.getWidth() + 3, refresh.getHeight() + 3,
                    BufferedImage.SCALE_DEFAULT), refreshBounds.x + 3, refreshBounds.y + 3, null);
        }
        else {
            g2d.drawImage(refresh, refreshBounds.x, refreshBounds.y, null);
        }

        if (homeBounds.contains(mouseX, mouseY)) {
            g2d.drawImage(homeButton[0], homeBounds.x, homeBounds.y, null);
        }
        else {
            g2d.drawImage(homeButton[1], homeBounds.x, homeBounds.y, null);
        }

        if(gamePaused) {
            String gamePausedString = "GAME PAUSED";
            g2d.setColor(Color.WHITE);
            g2d.setFont(new Font("Georgia", Font.BOLD, 30));
            g2d.drawString(gamePausedString, 35, Tetris.Controller.Window.HEIGHT / 2);
        }

        if(gameOver) {
            String gameOverString = "GAME OVER";
            g2d.setColor(Color.WHITE);
            g2d.setFont(new Font("Georgia", Font.BOLD, 30));
            g2d.drawString(gameOverString, 50, Tetris.Controller.Window.HEIGHT / 2);

            if (recordSaveAllow) {
                recordSaveAllow = !recordSaveAllow;
                try {
                    CsvWriter csvWriter = new CsvWriter(playerName, score);
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                return;
            }
        }

        g2d.setColor(Color.white);
        g2d.setFont(new Font("Georgia", Font.BOLD, 20));

        g2d.drawString(playerName, Tetris.Controller.Window.WIDTH - 125, Tetris.Controller.Window.HEIGHT / 2 - 60);
        g2d.drawString("SCORE", Tetris.Controller.Window.WIDTH - 125, Tetris.Controller.Window.HEIGHT / 2 - 30);
        g2d.drawString(score+"", Tetris.Controller.Window.WIDTH - 125, Window.HEIGHT / 2);

        g2d.setStroke(new BasicStroke(2));
        g2d.setColor(Color.black);

        for (int i = 0; i <= boardHeight; i++)
        {
            g2d.drawLine(0, i*blockSize, boardWidth*blockSize, i*blockSize);
        }

        for (int j = 0; j <= boardWidth; j++)
        {
            g2d.drawLine(j*blockSize, 0, j * blockSize, boardHeight * 30);
        }
    }

    // for creating new shape
    public void setNextShape() {
        int index = (int)(Math.random() * shapes.length);
        nextShape = new Shape(shapes[index].getCoords(), shapes[index].getBlock(), this, shapes[index].getColor());
    }

    // creating shape that was created in advance
    public void setCurrentShape(){
        currentShape = nextShape;
        setNextShape();

        for (int row = 0; row < currentShape.getCoords().length; row ++) {
            for (int col = 0; col < currentShape.getCoords()[0].length; col ++) {
                if (currentShape.getCoords()[row][col] != 0) {
                    if(board[currentShape.getY() + row][currentShape.getX() + col] != 0)
                        gameOver = true;
                }
            }
        }
    }

    //doing every cycle iteration
    class GameLooper implements ActionListener {

        @Override
        public void actionPerformed(ActionEvent e) {
            update();
            repaint();
        }
    }

    //for first start
    public void startGame() {
        restartGame();
        setNextShape();
        setCurrentShape();
        gameOver = false;
        looper.start();

    }

    //for restart game
    public void restartGame() {
        score = 0;

        for(int row = 0; row < board.length; row++) {
            for(int col = 0; col < board[row].length; col ++) {
                board[row][col] = 0;
            }
        }

        looper.stop();
    }

    public void addScore() {
        score++;
    }

    public int[][] getBoard() {
        return board;
    }

    //for set player name from Title's text line
    public void setPlayerName(String playerName) {
        this.playerName = playerName;
    }

    // buttons reaction
    @Override
    public void keyPressed(KeyEvent e) {

        if(e.getKeyCode() == KeyEvent.VK_UP || e.getKeyCode() == KeyEvent.VK_W)
            currentShape.rotateShape();

        if(e.getKeyCode() == KeyEvent.VK_RIGHT || e.getKeyCode() == KeyEvent.VK_D)
            currentShape.setDeltaX(1);

        if(e.getKeyCode() == KeyEvent.VK_LEFT || e.getKeyCode() == KeyEvent.VK_A)
            currentShape.setDeltaX(-1);

        if(e.getKeyCode() == KeyEvent.VK_DOWN || e.getKeyCode() == KeyEvent.VK_S)
            currentShape.speedUp();
    }

    @Override
    public void keyReleased(KeyEvent e) {
        if(e.getKeyCode() == KeyEvent.VK_DOWN || e.getKeyCode() == KeyEvent.VK_S)
            currentShape.speedDown();
    }
    @Override
    public void keyTyped(KeyEvent e) {

    }

    //mouse reaction
    @Override
    public void mouseDragged(MouseEvent e) {
        mouseX = e.getX();
        mouseY = e.getY();
    }
    @Override
    public void mouseMoved(MouseEvent e) {
        mouseX = e.getX();
        mouseY = e.getY();
    }
    @Override
    public void mouseClicked(MouseEvent e) {

    }
    @Override
    public void mousePressed(MouseEvent e) {
        if (e.getButton() == MouseEvent.BUTTON1)
            leftClick = true;
    }
    @Override
    public void mouseReleased(MouseEvent e) {
        if (e.getButton() == MouseEvent.BUTTON1)
            leftClick = false;
    }
    @Override
    public void mouseEntered(MouseEvent e) {

    }
    @Override
    public void mouseExited(MouseEvent e) {

    }
}