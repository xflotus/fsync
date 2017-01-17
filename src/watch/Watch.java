package watch;

import java.util.*;

public abstract class Watch {
	protected String base;
	protected Map<Integer, String> watches;
	protected Map<String, Integer> pathes;
	protected Queue<Wevent> eventQue;
	protected boolean isOn;

	public Watch(String base) {
		this.base = base;
		this.watches = new HashMap<Integer, String>();
		pathes = new HashMap<String, Integer>();
		this.eventQue = new LinkedList<Wevent>();
		isOn = false;
	}

	public abstract void start() throws WatchException;

	public abstract void stop() throws WatchException;

	public abstract Notify fetch() throws WatchException;
}
