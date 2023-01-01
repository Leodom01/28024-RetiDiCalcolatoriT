package EsJavaDataGram;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.ServerSocket;

public class Server {
	
	public static String getLine(String filename, int line) {
		String result = null;
		BufferedReader buffR;
		try {
			buffR = new BufferedReader(new FileReader(filename));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			return null;
		}
		int currentLine = 0;
		try {
			while((result = buffR.readLine()) != null) {
				if(line == currentLine) {
					buffR.close();
					return result;
				}
				currentLine++;
			}
			buffR.close();
			return null;
		} catch (IOException e) {
			e.printStackTrace();
			return null;
		}
	}

	public static void main(String args[]) {
		
		try {
			DatagramSocket socket = new DatagramSocket(9000);
			byte[] buffer = new byte[1024];
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
			
			while(true) {
				packet.setData(buffer);
				socket.receive(packet);
				
				ByteArrayInputStream bInStr = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
				DataInputStream dInStream = new DataInputStream(bInStr);
				
				String request = dInStream.readUTF();
				String file = request.split(" ")[0];
				String line = request.split(" ")[1];
				String toReturn = getLine(file, Integer.parseInt(line));
				System.out.println("Linea in uscita: "+toReturn);
				if(toReturn == null) {
					toReturn = "Risposta non available";
				}
				
				ByteArrayOutputStream bOutStr = new ByteArrayOutputStream();
				DataOutputStream dOutStr = new DataOutputStream(bOutStr);
				dOutStr.writeUTF(toReturn);
				byte[] data = bOutStr.toByteArray();
				
				packet.setData(data, 0, data.length);
				socket.send(packet);
				
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			
			e.printStackTrace();
		}
		
	}
	
}
