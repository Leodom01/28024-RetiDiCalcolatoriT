package es5_EXT;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Serializable;

public class Client {

	public static void main(String args[]) {
		String registryHost = null;
		int registryPort = 1099;
		
		if(args.length == 1) {
			registryHost = args[0];
		}else if(args.length == 2) {
			registryHost = args[0];
			registryPort = Integer.parseInt(args[1]);
		}else {
			System.out.println("Evoca il comando con: Client host_registry [porta_registry]");
			System.exit(1);
		}
		
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
		
		try {
			RemOp serverRmi = (RemOp)java.rmi.Naming.lookup("//"+registryHost+":"+registryPort+"/RemoteOps");
			
			String userString;
			System.out.println("Digita 1 per conta righe, 2 per elimina righe, CTRL+D per uscire:");
			while((userString = stdIn.readLine()) != null) {
				if(userString.equals("1")) {
					String filename;
					int maxWords;
					System.out.println("Dammi nome file:");
					filename = stdIn.readLine();
					System.out.println("Dammi max numero di parole per riga:");
					maxWords = Integer.parseInt(stdIn.readLine());
					System.out.println("Il server ha ritornato file con righe oltre il numero: "+serverRmi.conta_righe(filename, maxWords));
				}else if(userString.equals("2")) {
					String filename;
					int rowToDel;
					System.out.println("Dammi nome file:");
					filename = stdIn.readLine();
					System.out.println("Dammi il numero della riga da eliminare:");
					rowToDel = Integer.parseInt(stdIn.readLine());
					Serializable[] returnedVals = serverRmi.elimina_riga(filename, rowToDel);
					String filenameReturned = (String)returnedVals[0];
					int fileRowsReturned = (int)returnedVals[1];
					System.out.println("Elimina riga ha creato il nuovo file "+filenameReturned+" con numero righe: "+fileRowsReturned);
				}
				System.out.println("Digita 1 per conta righe, 2 per elimina righe, 3 per uscire:");
			}
		}catch(Exception e) {
			System.err.println("Errore: "+ e.getMessage());
			e.printStackTrace();
			try {
				stdIn.close();
			} catch (IOException e1) {
				e1.printStackTrace();
			}
			System.exit(2);
		}
		
		System.out.println("Tutto terminato correttamente, ciao ciao...");
		
		try {
			stdIn.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.exit(0);
	}
	
}
