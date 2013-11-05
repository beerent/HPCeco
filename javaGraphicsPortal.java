import java.io.*;
import javax.swing.*;
public class javaGraphicsPortal extends Thread{
	private String name;
	private final int PORT = 9999;
	private int x, y;

	public javaGraphicsPortal(){
		this.name = "Graphics Portal";
		this.x = -1;
		this.y = -1;
	}

	public javaGraphicsPortal(int x, int y){

	}

	public javaGraphicsPortal(String name, int x, int y){

	}

	//main method
	public static void main(String[] args) {
		javaGraphicsPortal jgp = new javaGraphicsPortal();
		jgp.createAndShowGUI();
    }

    //runs the javaGraphicsPortal as a Thread
    public void startPortal(){
    	ServerSocket ss = new ServerSocket(PORT);
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
        if(this.x <0 || this.y <0){
        	System.err.println("!!!coordinates not set!!!");
        	return;
        }
        frame.setVisible(true);
        frame.setResizable(false);
    }
}