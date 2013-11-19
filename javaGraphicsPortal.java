import java.net.*;
import javax.swing.*;
import java.io.*;
import java.util.*;

public class javaGraphicsPortal extends Thread{
    private final String CLIENTKEY = "clientKEY";
    
    /* used for while loop in runPortal() */
    private boolean portalOn;
    
    /* name for window, PORT number constant, x & y coordinates for window */
    private String name;
    private final int PORT = 9999;
    private int x, y;
    
    /* networking: out to client, in from client, socket connected to client */
    private PrintStream out;
    private BufferedReader in;
    private Socket s;
    
    /*imagePrinter object, to print to the window*/
    private imagePrinter ip;
    
    /*
      constructors:

      accepts nothing, an x & y coordinate, or x & y coordinate & window name
     */

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
    
    /*
      main method
     */

    public static void main(String[] args) {
	javaGraphicsPortal jgp = new javaGraphicsPortal("Brents World", 500, 500);
	//jgp.createAndShowGUI();
	jgp.startPortal();
    }
    
    /*
      this method begins the entire process of the javaGraphicsPortal 
     */

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
	    this.portalOn = true;
	    report("ecosystem is running.");
	    runPortal();
	    //this.start();
	}catch(Exception e){
	    System.out.println("failed connection attempt on port " + PORT);
	}
    }    
    
    /*
      communicates with the client, very primitive and pointless in this case, 
      but I feel it's good networking practice to handshake before communication.
    */
    
    private void handshake() throws IOException{
    	String key = in.readLine(); //get key from client
	if(!key.equals(CLIENTKEY)){
	    report("Received incorrect client key: " + key);
	    System.exit(1);
	}
	sendToClient("serverKEY"); //send key to client
	report("connection established");
    }
    
    /*
      communicates with the client receiving and storing the x and y coordinates, and name
      to the window they wish to create.

      creates a imagePrinter object for this class
     */

    private void buildImageBuffer() throws IOException{
        //get size of window
	int x = -1;
	int y = -1;
	
	String str1 = readFromClient();
	String str2 = readFromClient();
	String windowName = readFromClient();
	
	try{
	    //we use substring from 1 , because there's an extra space added to the beginning of everthing this
	    //server reveives. This is a security fix, or something of that matter. Can be turned off
	    //at a later date, should I want to. For now, factor in the space. 
	    
	    x = Integer.parseInt(str1.substring(1, str1.length()));
	    y = Integer.parseInt(str2.substring(1, str2.length()));
	    //x = Integer.parseInt(str1);
	    //y = Integer.parseInt(str2);
	}catch(Exception e){
	    System.out.println("invalid integer: cannot set bounds. abort.");
	    return;
	}
        //integers parsed
	report("creating window: " + x + " x " + y);
	this.ip = new imagePrinter(x, y, windowName);
	report("image Printer created.");
    }
    
    /*
      loops throughout the duration of the program, constantly
      sending the next input to the handleInput method.
     */
    
    private void runPortal() throws IOException{
	while(this.portalOn){
		String s = readFromClient();
		if(s != null)
	    	handleInput(s);
	}
    }

    /*
      method accepts input from the server in a separate thread, however 
      is currently unused.
     */
    
    public void run(){
		
		try{
	   Scanner sc;
	
	String coords = readFromClient();
	coords = coords.substring(1, coords.length());
	ip.fillCanvas(null);
	while(!coords.equals("OUT")){
		
	    sc = new Scanner(coords); //initialize Scanner
	    String id = sc.next();// skip id

	    int x = strtoint(sc.next()); // x
	    int y = strtoint(sc.next()); // y

	    if(x == -1 || y == -1){
		report("error receiving coordinates for creature: " + id);
	    }else{
		ip.setPoint(null, x, y);
	    }

	    coords = readFromClient();
	    coords = coords.substring(1, coords.length());
	}
	}catch(Exception e){
		System.out.println("FAIL IN THREAD");
	}
    }
    
    /*
      receives a String as a parameter, and analyzes the String
      to properly respond to the clients request.
     */
    
    private void handleInput(String input) throws IOException{
	input = input.substring(1, input.length());
	if(input.equals("IN")){
		printCoords();
	}else if(input.equals("done")){
	    try{
		s.close();
	    }catch(Exception e){
		System.exit(1);
	    }
	    System.exit(0);
	}else{
	    if(input!= null){
		report("size: " + input.length());
		report("Server got: " + input);
	    }
	}
    }

    /*
      once this method is called, the server reads input from the client 
      until the String "OUT" is received. 

      while in the while loop, coordinates are received, parsed, and send to the
      imagePrinter object to print them to the window.
     */

    private void printCoords()throws IOException{
	Scanner sc;
	
	String coords = readFromClient();
	coords = coords.substring(1, coords.length());
	ip.fillCanvas(null);
	int count = 0;
	while(!coords.equals("OUT")){
		
	    sc = new Scanner(coords); //initialize Scanner
	    String id = sc.next();// skip id

	    int x = strtoint(sc.next()); // x
	    int y = strtoint(sc.next()); // y

	    if(x == -1 || y == -1){
		report("error receiving coordinates for creature: " + id);
	    }else{
		ip.setPoint(null, x, y);
	    }

	    //report(""+x+y);
	    coords = readFromClient();
	    coords = coords.substring(1, coords.length());
	}
	ip.repaint();
    }

    /*
      simple method to parse a String to an int, so I don't need to write try catch blocks
      all over my program.

      returns n if parsable, else -1
     */

    private int strtoint(String s){
	try{
	    int n = Integer.parseInt(s);
	    return n;
	}catch(Exception e){
	    return -1;
	}
    }
    
    /*
      method that easily sends a statement to the client
     */

    private void sendToClient(String msg){
	out.println(msg);
    }
    
    /*
      method that easily reads and returns input from the client
     */

    private String readFromClient() throws IOException{
        String input = in.readLine();
	return input;
    }
    
    /*
      shorter way to print to console, also concat's a "S:" in front, to show which program
      is printing the statement
    */

    private void report(String s){
	System.out.println("S: " + s);
    }
}
