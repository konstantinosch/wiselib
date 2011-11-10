package ishell.plugins;

import org.eclipse.ui.IMemento;

import ishell.device.MessagePacket;
import ishell.gui.views.PluginView;
import ishell.plugins.Plugin;

import java.io.*;
import java.net.*;
//import java.util.*;

public class TrackingVisPlugin extends Plugin {
	Socket echoSocket = null;
	PrintWriter out = null;
	
	
	public TrackingVisPlugin(PluginView parent) {
		super(parent);
		// TODO Auto-generated constructor stub
	}
	

	@Override
	public void restoreData(IMemento arg0) {
		// TODO Auto-generated method stub

	}

	@Override
	public void saveData(IMemento arg0) {
		// TODO Auto-generated method stub

	}

	@Override
	public int[] setTypes() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void shutdown() 
	{
		// TODO Auto-generated method stub

	}

	@Override
	public void receivePacket(MessagePacket arg0) {
		// TODO Auto-generated method stub
		packet = arg0;
	}
	
	public MessagePacket packet = null;
}