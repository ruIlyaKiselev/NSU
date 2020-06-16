package Tetris.Controller;

import Tetris.View.Board;
import Tetris.View.Title;

import javax.swing.JFrame;

public class Window {
    public static final int WIDTH = 475, HEIGHT = 636;

    private Board board;
    private Title title;
    private JFrame window;

    public Window() {

        window = new JFrame("Tetris");
        window.setSize(WIDTH, HEIGHT);
        window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        window.setLocationRelativeTo(null);
        window.setResizable(false);

        board = new Board(this);
        title = new Title(this);

        window.addKeyListener(board);
        window.addMouseMotionListener(title);
        window.addMouseListener(title);

        window.add(title);

        window.setVisible(true);
    }

    public void startTetris() {
        title.setVisible(false);
        board.setVisible(true);
        window.addMouseMotionListener(board);
        window.addMouseListener(board);
        window.add(board);
        board.startGame();
        board.setPlayerName(title.getPlayerName());
        window.revalidate();
        window.requestFocus();
    }

    public void showMenu() {
        board.setVisible(false);
        title.setVisible(true);
        window.requestFocus();
    }

    public static void main(String[] args) {
        new Window();
    }
}