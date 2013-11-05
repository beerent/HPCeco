import java.io.*;
import javax.swing.*;
public class javaGraphicsPortal extends Thread{
	private String name;
	private int x, y;

	public javaGraphicsPortal(){
		this.name = "Graphics Portal";
	}

	public static void main(String[] args) {

		javaGraphicsPortal jgp = new javaGraphicsPortal();
		jgp.createAndShowGUI();
        //Schedule a job for the event-dispatching thread:
        //creating and showing this application's GUI.
        //javax.swing.SwingUtilities.invokeLater(new Runnable() {
        //    public void run() {
        //        createAndShowGUI();
        //    }
        //});
    }

    //sets the x and y axis for the JFrame
    public void setCoordinates(int x, int y){
    	this.x = x;
    	this.y = y;
    }

    //set the name for the frame. If not set, set to default String in constructor
    public void setWindowName(String name){
    	this.name = name;
    }

    /**
     * Create the GUI and show it.  For thread safety,
     * this method should be invoked from the
     * event-dispatching thread.
     */
    private void createAndShowGUI() {
        //Create and set up the window.
        JFrame frame = new JFrame(this.name);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        //Add the ubiquitous "Hello World" label.
        JLabel label = new JLabel("Hello World");
        frame.getContentPane().add(label);

        //Display the window.
        frame.setSize(this.x, this.y);
        frame.setVisible(true);
        frame.setResizable(false);
    }
}