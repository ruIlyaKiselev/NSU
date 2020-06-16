package Tetris.View;

import Tetris.Controller.Window;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferedImage;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.*;


public class Title extends JPanel implements MouseListener, MouseMotionListener {

    private int mouseX, mouseY;
    private boolean leftClick = false;

    private Rectangle playBounds;
    private Rectangle helpBounds;
    private Rectangle exitBounds;
    private Rectangle manualBounds;

    private BufferedImage title;
    private BufferedImage play;
    private BufferedImage help;
    private BufferedImage exit;
    private BufferedImage[] playButton = new BufferedImage[2];
    private BufferedImage manual;

    private JTextField nameField;
    private String playerName;
    private boolean showManual = false;

    private Timer buttonLapse = new Timer(1000, new ActionListener() {

        @Override
        public void actionPerformed(ActionEvent e) {
            buttonLapse.stop();
        }});

    private Tetris.Controller.Window window;

    public Title(Tetris.Controller.Window window) {
        try {
            title = ImageIO.read(Board.class.getResource("/Title.png"));
            play = ImageIO.read(Board.class.getResource("/play.png"));
            help = ImageIO.read(Board.class.getResource("/question.png"));
            exit = ImageIO.read(Board.class.getResource("/exit.png"));
            manual = ImageIO.read(Board.class.getResource("/manual.png"));

        } catch (IOException e) {
            e.printStackTrace();
        }

        mouseX = 0;
        mouseY = 0;

        playButton[0] = play.getSubimage(0, 0, 100, 80);
        playButton[1] = play.getSubimage(100, 0, 100, 80);

        setLayout(null);

        nameField = new JTextField("Player");
        nameField.setBounds(Tetris.Controller.Window.WIDTH / 2 - 70, Tetris.Controller.Window.HEIGHT / 2,
                140, 50);
        nameField.setFont(new Font("Dialog", Font.PLAIN, 30));
        nameField.setHorizontalAlignment(JTextField.CENTER);
        nameField.setForeground(Color.yellow);
        nameField.setBackground(new Color(68, 38, 117));
        add(nameField);

        playBounds = new Rectangle(Tetris.Controller.Window.WIDTH / 2 - 40, Tetris.Controller.Window.HEIGHT - 175,
                100, 80);

        helpBounds = new Rectangle(Tetris.Controller.Window.WIDTH / 2 + 100, Tetris.Controller.Window.HEIGHT - 150,
                help.getWidth(), help.getHeight());

        exitBounds = new Rectangle(Tetris.Controller.Window.WIDTH / 2 - 100 - exit.getWidth(),
                Tetris.Controller.Window.HEIGHT - 150, exit.getWidth(), exit.getHeight());

        manualBounds = new Rectangle(0, 0, manual.getWidth(), manual.getHeight());

        this.window = window;
    }

    public void paintComponent(Graphics g) {
        super.paintComponent(g);

        if (leftClick && playBounds.contains(mouseX, mouseY) && !showManual && !buttonLapse.isRunning()) {
            buttonLapse.start();
            playerName = nameField.getText();
            window.startTetris();
        }

        if (leftClick && helpBounds.contains(mouseX, mouseY) && !showManual && !buttonLapse.isRunning()) {
            buttonLapse.start();
            nameField.setVisible(false);
            showManual = !showManual;
        }

        if (leftClick && manualBounds.contains(mouseX, mouseY) && showManual && !buttonLapse.isRunning()) {
            buttonLapse.start();
            showManual = !showManual;
        }

        if (!showManual) {
            nameField.setVisible(true);
        }

        if (leftClick && exitBounds.contains(mouseX, mouseY) && !showManual && !buttonLapse.isRunning()) {
            System.exit(0);
        }

        g.setColor(Color.darkGray);
        g.fillRect(0, 0, Tetris.Controller.Window.WIDTH, Tetris.Controller.Window.HEIGHT);

        g.drawImage(title, Tetris.Controller.Window.WIDTH / 2 - title.getWidth() / 2,
                Tetris.Controller.Window.HEIGHT / 2 - title.getHeight() / 2 - 200, null);

        if(playBounds.contains(mouseX, mouseY)) {
            g.drawImage(playButton[0], Tetris.Controller.Window.WIDTH / 2 - 50,
                    Tetris.Controller.Window.HEIGHT - 200, null);
            repaint();
        }
        else {
            g.drawImage(playButton[1], Tetris.Controller.Window.WIDTH / 2 - 50, Window.HEIGHT - 200,
                    null);
            repaint();
        }

        if (helpBounds.contains(mouseX, mouseY)) {
            g.drawImage(help.getScaledInstance(help.getWidth(), help.getHeight(),
                    BufferedImage.SCALE_DEFAULT), helpBounds.x + 3, helpBounds.y + 3, null);
            repaint();
        }
        else {
            g.drawImage(help, helpBounds.x, helpBounds.y, null);
            repaint();
        }

        if (exitBounds.contains(mouseX, mouseY)) {
            g.drawImage(exit.getScaledInstance(exit.getWidth(), exit.getHeight(),
                    BufferedImage.SCALE_DEFAULT), exitBounds.x + 3, exitBounds.y + 3, null);
            repaint();
        }
        else {
            g.drawImage(exit, exitBounds.x, exitBounds.y, null);
            repaint();
        }

        if (showManual) {
            g.drawImage(manual, manualBounds.x, manualBounds.y, null);
            repaint();
        }
    }

    public String getPlayerName() {
        return playerName;
    }

    @Override
    public void mouseClicked(MouseEvent e) {
    }

    @Override
    public void mousePressed(MouseEvent e) {
        if(e.getButton() == MouseEvent.BUTTON1)
            leftClick = true;
    }

    @Override
    public void mouseReleased(MouseEvent e) {
        if(e.getButton() == MouseEvent.BUTTON1)
            leftClick = false;
    }

    @Override
    public void mouseEntered(MouseEvent e) {

    }

    @Override
    public void mouseExited(MouseEvent e) {
    }

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
}