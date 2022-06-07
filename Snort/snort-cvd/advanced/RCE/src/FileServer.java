/* Decompiler 35ms, total 163ms, lines 163 */
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Calendar;
import javax.net.ServerSocketFactory;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

public class FileServer {
   private final int PORT = 7777;
   private final int USER_LIMIT = 100;
   public static final int VALID = 0;
   public static final int BAD_CREDENTIALS = 1;
   public static final int NO_PERMISSION = 2;
   public static final int TIME_PERMISSION = 3;
   private int userCount;
   private ServerSocket server;
   private String[] userList = new String[100];
   private String[] passwordList = new String[100];

   FileServer() {
      try {
         this.getUsers();
         this.server = ServerSocketFactory.getDefault().createServerSocket(7777);
      } catch (Exception var2) {
         var2.printStackTrace();
      }

   }

   public static void main(String[] args) {
      FileServer instance = new FileServer();
      instance.listener();
   }

   public void listener() {
      while(true) {
         try {
            Socket socket = this.server.accept();
            new ConnectionHandler(this, socket);
         } catch (IOException var2) {
            var2.printStackTrace();
         }
      }
   }

   public void getUsers() {
      File inputFile = new File("." + File.separator + "users.txt");
      this.userCount = 0;

      try {
         boolean isUser = true;
         BufferedReader input = new BufferedReader(new FileReader(inputFile));

         String data;
         while((data = input.readLine()) != null) {
            if (isUser) {
               this.userList[this.userCount] = data.toLowerCase();
               isUser = false;
            } else {
               this.passwordList[this.userCount] = data;
               isUser = true;
               ++this.userCount;
               if (this.userCount > 100) {
                  break;
               }
            }
         }

         for(int i = 0; i < this.userCount; ++i) {
            for(int j = i + 1; j < this.userCount; ++j) {
               if (this.userList[i].compareTo(this.userList[j]) > 0) {
                  String username = this.userList[i];
                  String password = this.passwordList[i];
                  this.userList[i] = this.userList[j];
                  this.passwordList[i] = this.passwordList[j];
                  this.userList[j] = username;
                  this.passwordList[j] = password;
                  System.out.println(userList[i]);
               }
            }
         }

         System.out.print(this.userCount + " users are allowed access to the system.\n");
      } catch (Exception var9) {
         var9.printStackTrace();
      }

   }

   protected int verifyData(String username, String password, String fileName) {
      int status = 1;
      Calendar time = Calendar.getInstance();
      long currentTime = 0L;

      try {
         currentTime = time.getTimeInMillis();
         System.out.println("The current time is: " + currentTime);
      } catch (Exception var17) {
         var17.printStackTrace();
      }

      for(int i = 0; i < this.userCount; ++i) {
         if (username.equals(this.userList[i])) {
            if (password.equals(this.passwordList[i])) {
               status = 2;
            }
            break;
         }
      }

      if (status == 1) {
         return status;
      } else {
         File file = new File(fileName.substring(0, fileName.length() - 4).concat(".xml"));

         try {
            Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(file);
            NodeList nodeList = doc.getElementsByTagName("user");

            for(int i = 0; i < nodeList.getLength(); ++i) {
               Element current = (Element)nodeList.item(i);
               if (username.replace(" ", "").equals(current.getElementsByTagName("username").item(0).getFirstChild().getTextContent().replace(" ", ""))) {
                  long after = Long.parseLong(current.getElementsByTagName("after").item(0).getFirstChild().getTextContent());
                  long before = Long.parseLong(current.getElementsByTagName("before").item(0).getFirstChild().getTextContent());
                  if (currentTime > after && currentTime < before) {
                     status = 0;
                  } else {
                     status = 3;
                  }
                  break;
               }
            }

            return status;
         } catch (Exception var18) {
            var18.printStackTrace();
            return 1;
         }
      }
   }

   protected String getProperty(String fileName, String property) {
      String output = "";
      File file = new File(fileName);

      try {
         Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(file);
         Element current = (Element)doc.getElementsByTagName("properties").item(0);
         current = (Element)current.getElementsByTagName(property).item(0);
         output = current.getFirstChild().getTextContent();
      } catch (Exception var8) {
         var8.printStackTrace();
      }

      return output;
   }
}
