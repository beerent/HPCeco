import java.net.*;
import javax.swing.*;
import java.io.*;

public class javaGraphicsPortal extends Thread{
    private final String CLIENTKEY = "clientKEY";

    private boolean portalOn;

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
         this.portalOn = true;
         runPortal();
     }catch(Exception e){
         System.out.println("failed connection attempt on port " + PORT);
     }
 }    

    //accepts a socket s, which established a connection 
 private void handshake() throws IOException{
    	String key = readFromClient(); //get key from client
     if(!key.equals(CLIENTKEY)){
         report("Received incorrect client key: " + key);
         System.exit(1);
     }
	sendToClient("serverKEY"); //send key to client
	report("connection established");
}

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
    }catch(Exception e){
        System.out.println("invalid integer: cannot set bounds. abort.");
        return;
    }
        //integers parsed
    report("creating window: " + x + " x " + y);
    this.ip = new imagePrinter(x, y, windowName);
    report("image Printer created.");
}

private void runPortal() throws IOException{
    while(this.portalOn){
        handleInput(readFromClient());
        this.portalOn = false;
    }
}

private void handleInput(String input){

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
