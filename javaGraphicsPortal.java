import java.net.*;
import javax.swing.*;
import java.io.*;
public class javaGraphicsPortal extends Thread{
    private final String CLIENTKEY = "clientKEY";

    private String name;
    private final int PORT = 9999;
    private int x, y;
    
    private PrintWriter out;
    private BufferedReader in;
    
    public javaGraphicsPortal(){
	this.name = "Graphics Portal";
	this.x = -1;
	this.y = -1;
    }
    
    public javaGraphicsPortal(int x, int y){
	this.name = "Graphics Portal";
	this.x = x;
	this.y = y;
    }
    
    public javaGraphicsPortal(String name, int x, int y){
	this.name = name;
	this.x = x;
	this.y = y;
    }
    
    //main method
    public static void main(String[] args) {
	javaGraphicsPortal jgp = new javaGraphicsPortal("Brents World", 200, 200);
	jgp.startPortal();
    }
    
    //runs the javaGraphicsPortal as a Thread
    public void startPortal(){
    	ServerSocket ss;
    	Socket s;
    	try{
	    ss = new ServerSocket(PORT);
    	}catch(Exception e){
	    System.out.println("cannot connect to port " + PORT);
	    return;
    	}
    	try{
	    //waiting for connection
	    System.out.println("waiting for connection...");
	    s = ss.accept();
	    //connection established
	    System.out.println("Connected on port " + PORT);
	    
	    //set up server IO to client
	    this.out = new PrintWriter(s.getOutputStream(), true);
	    this.in = new BufferedReader(new InputStreamReader(s.getInputStream()));
	    
	    //out.println("test");
	    //out.println("test2");
	    
	    handshake(s);
	    setParameters(s);
	}catch(Exception e){
	    System.out.println("failed connection attempt on port " + PORT);
	}
    }
    
    private void handshake(Socket s) throws IOException{
    	String key = in.readLine();             //get key from client
	if(!key.equals(CLIENTKEY)){
	    System.out.println("Received incorrect client key");
	    return;
	} 
    	out.println("serverKEY");                     //send key to client
    }
    
    private void setParameters(Socket s){
	
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
