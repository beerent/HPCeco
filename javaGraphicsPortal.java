import java.net.*;
import javax.swing.*;
import java.io.*;

public class javaGraphicsPortal extends Thread{
    private final String CLIENTKEY = "clientKEY";

    private String name;
    private final int PORT = 9999;
    private int x, y;
    
    private PrintStream out;
    private BufferedReader in;
    private Socket s;

    private imagePrinter ip;
    
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
	javaGraphicsPortal jgp = new javaGraphicsPortal("Brents World", 500, 500);
	//jgp.createAndShowGUI();
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
	    report("waiting for connection...");
	    s = ss.accept();
	    //connection established
	    report("Connected on port " + PORT);
	    
	    //set up server IO to client
	    this.s = s;
	    this.out = new PrintStream(s.getOutputStream(), true);
	    this.in = new BufferedReader(new InputStreamReader(s.getInputStream()));
	    
	    handshake();
	    buildImageBuffer();
	    setParameters();
	}catch(Exception e){
	    System.out.println("failed connection attempt on port " + PORT);
	}
    }
    
    private void buildImageBuffer() throws IOException{
        //get size of window
        int x = -1;
        int y = -1;

        String str1 = readFromClient();
        String str2 = readFromClient();

	try{
	    //we use substring from 1 , because there's an extra space added to the beginning of everthing this
	    //server reveives. This is a security fix, or something of that matter. Can be turned off
	    //at a later date, should I want to. For now, factor in the space. 
            x = Integer.parseInt(str1.substring(1, str1.length()));
            y = Integer.parseInt(str2.substring(1, str2.length()));
        }catch(Exception e){
            System.out.println("invalid integer: cannot set bounds. abort.");
            return;
        }
        //integers parsed
        this.ip = new imagePrinter(x, y);
	report("image Printer created.");
    }
    
    
    //accepts a socket s, which established a connection 
    private void handshake() throws IOException{
    	String key = readFromClient(); //get key from client
	if(!key.equals(CLIENTKEY)){
	    report("Received incorrect client key: " + key);
	    return;
	}
	sendToClient("serverKEY"); //send key to client
	report("connection established");
    }
    
    private void setParameters(){
	
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

    private void sendToClient(String msg){
	out.println(msg);
    }

    private String readFromClient() throws IOException{
	return in.readLine();
    }

    private void report(String s){
	System.out.println("S: " + s);
    }
}
