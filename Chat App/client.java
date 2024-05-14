import java.net.*;
import java.io.*;
class client {
    public static void main(String a[]) throws Exception
    {
        System.out.println("Client Application is running....");

        System.out.println("Clien is waiting for conncetion ");
        Socket s=new Socket("localhost",2100);

        PrintStream ps=new PrintStream(s.getOutputStream());
 
        BufferedReader br1=new BufferedReader(new InputStreamReader(s.getInputStream()));
        BufferedReader br2=new BufferedReader(new InputStreamReader(System.in));

        System.out.println("Chat messanger started");
        String str1,str2;
        

        while(!(str1 = br2.readLine()).equals("end"))
        {
            ps.println(str1);
            System.out.println("Enter message for sever :");
            str2 = br1.readLine();
            System.out.println("Server says :"+str2);
        }
        System.out.println("Thanks for using Char Messager ....");
    }
}
