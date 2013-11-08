import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class imagePrinter extends JPanel {
    private int width, height;    
    private BufferedImage window;

    public imagePrinter(int width, int height) {
	this.width = width;
	this.height = height;
        this.window = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
    }

    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2 = (Graphics2D) g;
        g2.drawImage(this.window, null, null);
    }

    public void fillCanvas(Color c) {
        int color = c.getRGB();
        for (int x = 0; x < this.window.getWidth(); x++) {
            for (int y = 0; y < this.window.getHeight(); y++) {
                this.window.setRGB(x, y, color);
            }
        }
        repaint();
    }

    public void drawLine(Color c, int x1, int y1, int x2, int y2) {
        // Implement line drawing
        repaint();
    }

    public void drawRect(Color c, int x1, int y1, int width, int height) {
        int color = c.getRGB();
        // Implement rectangle drawing
        for (int x = x1; x < x1 + width; x++) {
            for (int y = y1; y < y1 + height; y++) {
                this.window.setRGB(x, y, color);
            }
        }
        repaint();
    }

    public void drawOval(Color c, int x1, int y1, int width, int height) {
        // Implement oval drawing
        repaint();
    }

    public static void main(String[] args) {
        int width = 640;
        int height = 480;
        JFrame frame = new JFrame("Direct draw demo");

        imagePrinter panel = new imagePrinter(width, height);

        frame.add(panel);
        frame.pack();
        frame.setVisible(true);
        frame.setResizable(false);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
}
