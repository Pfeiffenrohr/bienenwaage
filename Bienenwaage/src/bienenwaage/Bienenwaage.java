package bienenwaage;

import com.pi4j.io.serial.*;
import com.pi4j.util.CommandArgumentParser;
import com.pi4j.util.Console;

import java.util.Vector;
import java.io.IOException;
import java.util.Date;
import java.util.Hashtable;

import common.FileHandling;

public class Bienenwaage {

	Vector queue = new Vector();
	Hashtable env = new Hashtable();

	public static void main(String args[]) throws InterruptedException,
			IOException {
		final Serial serial = SerialFactory.createInstance();
		// final String filename = "/tmp/arduino.out";
		FileHandling fh = new FileHandling();
		if (args.length < 1) {
			System.err
					.println("Configfile is missing. Ussage bienenwaage <configfile>");
			System.exit(1);
		}
		String configfile = args[0];
		System.out.println("Read configfile " + configfile);
		Hashtable config = fh.readFileAsHashtable(configfile);
		Bienenwaage bienenwaage = new Bienenwaage();
		bienenwaage.init(config);
	}

	private void init(Hashtable config) {
		env = config;
		ReadSerial serial = new ReadSerial(queue, env);
		CommunicationConsole cons = new CommunicationConsole(env);

		env.put("debug", "10"); // kein Debug
		env.put("maxQueue", 0);
		env.put("iterationen", 0);
		env.put("aktSensor", 0);
		env.put("maxSensor", 0);
		env.put("mTChanged", "0");
		env.put("mT", "10");
		env.put("messageChanged", "0");
		env.put("message", "");
		env.put("statMode", "unknown");
		env.put("statAuf", "unknown");

		// Startzeit
		Date start = new Date();

		env.put("startzeit", start);

		Thread read = new Thread(serial);
		

		serial.queue = queue;
		read.start();
	

		parseQueue(queue);

	}

	private void parseQueue(Vector queue) {
		String str;
		FileHandling fh = new FileHandling();
		final String filename = "/tmp/bienenwaage.txt";
		String debug = null;
		int maxQueue = 0;
		int maxSensor = 0;
		// DB db = new DB();
		DBConnect connect = new DBConnect(env);

		while (true) {
			try {
				// System.out.println("Parse Queue");
				debug = (String) env.get("debug");
				// System.out.println("Debug = "+debug);
				if (queue.size() > maxQueue) {
					maxQueue = queue.size();
					env.put("maxQueue", maxQueue);
				}
				if (debug.equals("0")) {
					System.out.println("Länge der Queue: " + queue.size());
				}
				while (!queue.isEmpty()) {

					str = (String) queue.elementAt(0);
					// System.out.println(str);
					// Ermittle Sensorwet
					if (str.startsWith("Gewicht:")) {
						try {

							 System.out.println(">"+str.trim()+"<");
							String[] werte = str.split(" ");
							 System.out.println(">"+werte[1].trim()+"<1");
							 //String [] gewicht = werte[1].trim().split(" ");
							 //System.out.println("Nach Split");
							 //System.out.println("Gewicht in g "+gewicht[1]);
							 
							Integer val = new Integer(werte[1].trim());
							env.put("aktSensor", val.intValue());
							connect.insertWeight(val.intValue());
							if (val.intValue() > maxSensor) {
								maxSensor = val.intValue();
								env.put("maxSensor", maxSensor);
							}
						} catch (Exception ex) {
							System.err.println(ex.getStackTrace().toString());
						}
					}
					

					if (str.startsWith("Liessensor") && !debug.equals("0")) {
						queue.removeElementAt(0);
						continue;
					}
					if (str.startsWith("Iterationen") && !debug.equals("0")) {
						queue.removeElementAt(0);
						continue;
					}
					if (str.startsWith("Sensorwert") && !debug.equals("0")) {
						queue.removeElementAt(0);
						continue;
					}

					if (str.startsWith("Message") && !debug.equals("0")) {
						queue.removeElementAt(0);
						continue;
					}
					if (str.startsWith("Status") && !debug.equals("0")) {
						queue.removeElementAt(0);
						continue;
					}
					if (str.startsWith(" ") && !debug.equals("0")) {
						queue.removeElementAt(0);
						continue;
					}

					System.out.println(str.trim());

					fh.writeFile(filename,
							new Date().toString() + " " + str.trim(), true);
					// System.err.println(queue.elementAt(0));

					queue.removeElementAt(0);
				}
			} catch (Exception ex) {
				System.err.println(ex);
			}

			try {
				Thread.sleep(1000);
			} catch (Exception ex) {
				System.err.println(ex);
			}
		}

	}

}
