import java.awt.*;
import java.awt.Frame;
import java.awt.image.BufferedImage;
import javax.swing.*;

public class imagePrinter extends JPanel {

    private String name; /* name of window */
    private int width, height; /* x and y plane for the window */
    private BufferedImage window; /* window to print to */
    private Frame frame; /* */

    /**/
    public imagePrinter(int width, int height, String name) {
	this.width = width;
	this.height = height;
	this.window = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
	this.frame = new Frame(name);
	this.fillCanvas(Color.black);
	frame.setSize(width, height);
	frame.add(this);
	repaint();
	frame.setVisible(true);
	frame.setResizable(true);
	//frame.setDefaultCloseOperation(Frame.EXIT_ON_CLOSE);
    }
    
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2 = (Graphics2D) g;
        g2.drawImage(this.window, null, null);
    }

    public void fillCanvas(Color c) {
        if(c == null) c = Color.black;
        int color = c.getRGB();
        for (int x = 0; x < this.window.getWidth(); x++) {
            for (int y = 0; y < this.window.getHeight(); y++) {
                this.window.setRGB(x, y, color);
            }
        }
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

    public void setPoint(Color c, int x, int y){
	if(c == null) c = Color.red;
        if(x> this.width || y > this.height){
            System.out.println("point (" + x + ", " + y + ") is unreachable.");
            return;
        }
        this.window.setRGB(x, y, c.getRGB());
        //repaint();
    }

    public void drawOval(Color c, int x1, int y1, int width, int height) {
        // Implement oval drawing
        repaint();
    }

    public void loadWindow(){

    }

    public static void main(String[] args) {
        int width = 640;
        int height = 480;
        JFrame frame = new JFrame("Direct draw demo");

        imagePrinter panel = new imagePrinter(width, height, null);
        /*
        panel.drawRect(Color.black, 5, 5, 40, 40);
        panel.fillCanvas(Color.black);
        panel.setPoint(Color.red, 20, 20);
        */

        frame.add(panel);
        frame.pack();
        frame.setVisible(true);
        frame.setResizable(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
}
