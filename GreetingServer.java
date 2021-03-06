import java.net.*;
import java.io.*;

public class GreetingServer extends Thread
{
   /*
   This class uses a Thread in order to accept data from the android app.It makes use of Sockets
   */
   private ServerSocket serverSocket;
   String stickercolors="";
   public GreetingServer(int port) throws IOException
   {
      /*
      Constructor called to initialize variables
      */
      serverSocket = new ServerSocket(port);
      serverSocket.setSoTimeout(1000000);
   }

   public void run()
   {
      /*
      This function receives data, writes it to test.txt and closes the server on completion.
      */
      while(true)
      {
         try
         {
            System.out.println("Waiting for client on port " +
            serverSocket.getLocalPort() + "...");
            Socket server = serverSocket.accept();
            System.out.println("Just connected to "
                  + server.getRemoteSocketAddress());

            DataInputStream in =
                  new DataInputStream(server.getInputStream());
            
            stickercolors+=in.readUTF()+'\n';
            System.out.println(stickercolors);
      OutputStream os = new FileOutputStream("test.txt"); // Write to test.txt
      for(int x=0; x < stickercolors.length() ; x++){
         os.write(stickercolors.charAt(x));
      }
            DataOutputStream out =
                 new DataOutputStream(server.getOutputStream());
            out.writeUTF("Thank you for connecting to "
              + server.getLocalSocketAddress() + "\nGoodbye!");
            server.close();
             // writes the bytes
            os.close();
     
      if (stickercolors.length() > 30) // Close the server when fourth line comes
         break;
      
      
         }catch(SocketTimeoutException s)
         {
            System.out.println("Socket timed out!");
            break;
         }catch(IOException e)
         {
            e.printStackTrace();
            break;
         }
      }
   }
   public static void main(String [] args)
   {
      int port = Integer.parseInt(args[0]); // Port received via command line arguments
      try
      {
         Thread t = new GreetingServer(port); // Start a thread
         t.start();
      }catch(IOException e)
      {
         e.printStackTrace();
      }
   }
}