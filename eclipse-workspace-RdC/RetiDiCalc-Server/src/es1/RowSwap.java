package es1;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

public class RowSwap extends Thread{

	public int port; 
	public String filename;
	
	public String swapRow(String filename, int row1, int row2) {
		try {
			BufferedReader fileRdr = new BufferedReader(new FileReader(filename));
			FileWriter fileWr = new FileWriter("temp_"+filename);
			String firstRow = null, secondRow = null;
			String lineContent;
			int lineNum = 0;
			while((lineContent = fileRdr.readLine()) != null) {
				if(lineNum == row1) {
					firstRow = lineContent;
				}else if(lineNum == row2) {
					secondRow = lineContent;
				}
				fileWr.append(lineContent+"\n");
				lineNum++;
			}
			lineNum = 0;
			fileRdr.close();
			fileWr.close();
			
			fileRdr = new BufferedReader(new FileReader("temp_"+filename));
			fileWr = new FileWriter(filename);
			while((lineContent = fileRdr.readLine()) != null) {
				if(lineNum == row1) {
					fileWr.append(secondRow+"\n");
				}else if(lineNum == row2) {
					fileWr.append(firstRow+"\n");
				}else {
					fileWr.append(lineContent+"\n");
				}
				lineNum++;
			}
			fileRdr.close();
			fileWr.close();
			new File("temp_"+filename).delete();
			
			return "Great success!";
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			return "Great fail!";
		} catch (IOException e) {
			e.printStackTrace();
			return "Great fail!";
		}
		
	}
	
	public RowSwap(int port, String filename) {
		this.port = port; 
		this.filename = filename;
	}
	
	public void run() {
		System.out.println("Partito server per "+this.filename+" sulla porta: "+this.port);
		
		try {
			DatagramSocket socket = new DatagramSocket(this.port);
			byte[] data = new byte[1024];
			DatagramPacket packet = new DatagramPacket(data, data.length);
			
			while(true) {
				System.out.println(port+" pronto a ricevere altra merce...");
				packet.setData(data);
				socket.receive(packet);
				
				ByteArrayInputStream bInStr = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
				DataInputStream dInStream = new DataInputStream(bInStr);
				
				String request = dInStream.readUTF();
				System.out.println(port+" ricevuto "+request);
				int row1 = Integer.parseInt(request.split(" ")[0]);
				int row2 = Integer.parseInt(request.split(" ")[1]);
				
				String result = this.swapRow(filename, row1, row2);
				
				ByteArrayOutputStream bOutStr = new ByteArrayOutputStream();
				DataOutputStream dOutStr = new DataOutputStream(bOutStr);
				dOutStr.writeUTF(result);
				byte[] response = bOutStr.toByteArray();
				System.out.println(port + "Fillato risposta: "+result+" per porta "+packet.getPort());
				packet.setData(response);
				socket.send(packet);
				System.out.println(port+" mandato dato");
			}
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
}
