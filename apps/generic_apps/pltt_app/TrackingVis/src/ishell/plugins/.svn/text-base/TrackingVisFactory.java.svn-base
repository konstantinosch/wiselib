
package ishell.plugins;

import org.eclipse.ui.IMemento;
import org.eclipse.ui.IWorkbenchPage;

import ishell.gui.views.PluginView;
import ishell.plugins.Plugin;
import ishell.plugins.PluginDetailsPage;
import ishell.plugins.PluginFactory;
import ishell.plugins.PluginManager;
import ishell.plugins.PluginManagerView;

public class TrackingVisFactory extends PluginFactory {

	public static final String ID = "ishell.testTrackingVisFactory";
	public TrackingVisFactory() {
		setType(ID);
	}
	public Plugin addPlugin(IWorkbenchPage workbenchPage, PluginManager owner, IMemento memento) {
		return add(workbenchPage, owner, memento, TrackingVisView.ID,this);
	}
	@Override
	public Plugin createPlugin(PluginView pluginView) {
		return new TrackingVisPlugin(pluginView);
	}
	@Override
	public String getDescription() {
		return "adds a TrackingVisPlugin";
	}

	@Override
	public String getName() {
		// TODO Auto-generated method stub
		return "TrackingVisPlugin";
	}

	@Override
	public PluginDetailsPage getPluginManagerPage(String title, PluginManagerView view, IMemento memento, boolean isFactory) {
		return new TrackingVisPluginManagerPage(title,view,memento,isFactory);

	}
}
