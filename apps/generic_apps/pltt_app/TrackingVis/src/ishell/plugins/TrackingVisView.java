package ishell.plugins;



import ishell.gui.views.PluginView;
import ishell.plugins.Plugin;
import org.eclipse.swt.layout.*;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.*;
import org.eclipse.swt.events.*;
import java.util.Random;
import org.eclipse.ui.IMemento;
import org.eclipse.swt.graphics.*;


public class TrackingVisView extends PluginView {
	
	public static final String ID = "ishell.plugins.TrackingVis";
	//protected static Category log = Logging.get(TestView.class);
	private TrackingVisPlugin plugin = null;
	
	@Override
	public void addPlugin(Plugin plugin) {
		this.plugin = (TrackingVisPlugin) plugin;
	//	log.debug(plugin.getId());
		
	}

	@Override
	public void fillPage(Composite arg0)
	{
		// TODO Auto-generated method stub
		composite = arg0;
		//t = new Text(composite, SWT.NONE);
		c = new Canvas(composite, SWT.NONE);
		gridData = new GridData(GridData.FILL, GridData.FILL, true, true);
		c.setLayoutData(gridData);
		//t.setLayoutData(gridData);
		c.addPaintListener(new PaintListener() {
		      public void paintControl(PaintEvent e) {
		    	  if (image_location.equals("") == false )
		    	  {
		    		  image = new Image(composite.getDisplay(),image_location);
		    		  Image scaled_image = new Image(composite.getDisplay(), image.getImageData().scaledTo(c.getSize().x, c.getSize().y) );
		    		  e.gc.drawImage(scaled_image, 0, 0);
		    		  image.dispose();
		    		  scaled_image.dispose();
		    	  }
		          //e.gc.setBackground(e.display.getSystemColor(SWT.COLOR_BLACK));
		          //e.gc.fillRectangle(0, 0, c.getSize().x, c.getSize().y);
		    	  e.gc.setAlpha( target_alpha );
		    	  e.gc.setBackground(e.display.getSystemColor( SWT.COLOR_BLACK ) );
		          
		          int disp_x = 0;
		          int disp_y = 0;
				  if ( target_x >  testbed_dim_x - (c.getSize().x + c.getSize().y) /2*target_size_percent/100/2 )
				  {
				     disp_x = (c.getSize().x + c.getSize().y)/2*target_size_percent/100;
				  }
				  if (target_y > testbed_dim_y - (c.getSize().x + c.getSize().y)/2*target_size_percent/100/2 )
				  {
				     disp_y = (c.getSize().x + c.getSize().y)/2*target_size_percent/100;
				  }
		          e.gc.fillOval(c.getSize().x*target_x/testbed_dim_x - disp_x, c.getSize().y*target_y/testbed_dim_y - disp_y, (c.getSize().x + c.getSize().y)/2*target_size_percent/100 , (c.getSize().x + c.getSize().y)/2*target_size_percent/100 );
		          
		        }
		});
		if ( run_once == true )
		{
			R = new Random();
			run_once = false;
			composite.getDisplay().timerExec(1000, runnable);
		}
	}

	@Override
	public Plugin getPlugin() {
		return plugin;
	}

	@Override
	protected void refreshData() {
		// TODO Auto-generated method stub
	}

	@Override
	public void restoreData(IMemento arg0) {
		// TODO Auto-generated method stub
	}

	@Override
	public void saveData(IMemento arg0) {
		// TODO Auto-generated method stub
		
	}
	
	//---------------parameters----------//
	//private int testbed_dim_x=162; //unige_x
	//private int testbed_dim_y=162; //unige_y
	private int testbed_dim_x=345; //uzl_x
	private int testbed_dim_y=140; //uzl_y
	private boolean mirror_x = false;
	private boolean mirror_y = true;
	//private int target_size_percent=13; //unige target size
	private int target_size_percent=17;
	private int target_alpha = 90;
	private String image_location = "/home/konstantinos/Desktop/ishell2/TrackingVis/uzl_testbed.png";
	//----------------------------------//
	
	
	
	private Text t = null;
	private Image image = null;
	private Random R = null;
	private boolean run_once = true;
	private int target_x = 0;
	private int target_y = 0;
	private Composite composite;
	private Canvas c = null;
	private GridData gridData = null;
	private Runnable runnable = new Runnable()
	{
		public void run()
		{
			
			//Long l = new Long( plugin.packet.getId());
			//t.setText(l.toString());
			//target_x = R.nextInt(testbed_dim_x);
			//target_y = R.nextInt(testbed_dim_y);
			StringBuffer sb = new StringBuffer(plugin.packet.toString());
			int position_start = sb.lastIndexOf("hex[0x0 ") + 8;
			int position_end = sb.lastIndexOf("],");
			

			String filtered = sb.substring( position_start, position_end ).toString().replaceAll("0x3","").replaceAll(" ","").toString();
			
			position_start = filtered.indexOf("a");
			position_end = filtered.lastIndexOf("a");
			String node_id = filtered.substring(0, position_start);
			String coord_x = filtered.substring(position_start+1, position_end );
			String coord_y = filtered.substring(position_end+1, filtered.length() );
			//t.setText( node_id + ":" + coord_x + ":" + coord_y );
			
			target_x = Integer.parseInt(coord_x);
			target_y = Integer.parseInt(coord_y);
			
			//target_x = 345-7;
			//target_y = 105 - 7;
			
			if (mirror_x)
			{
				target_x = testbed_dim_x - target_x;
			}
			if (mirror_y)
			{
				target_y = testbed_dim_y - target_y;
			}
			c.redraw();
			composite.getDisplay().timerExec(10, this);
		}
	};
}
