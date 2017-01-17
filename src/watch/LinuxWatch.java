package watch;

import static watch.WatchException.*;

import java.io.File;
import java.util.*;

public class LinuxWatch extends Watch {
	private int fileDesc;

	public LinuxWatch(String base) {
		super(base.trim());
	}

	private boolean isValidDir(String pathName) {
		File f = new File(pathName);
		if (f.exists() && f.isDirectory())
			return true;
		else
			return false;
	}
	
	public void start() throws WatchException {
		if (!isOn) {
			System.loadLibrary("LinuxWatch");
			if (isValidDir(base)) {
				fileDesc = init();
				if (fileDesc == -1)
					throw new WatchException(FAIL_TO_CREATE_INOTIFIER);
				add(base, fileDesc, watches, pathes);
				isOn = true;
			} else
				throw new WatchException(INVALID_PATHNAME);
		}
	}

	public void stop() throws WatchException {
		if (isOn) {
			boolean ok = destory(fileDesc);
			if (ok)
				isOn = false;
			else
				throw new WatchException(FAIL_TO_RELEASE_INOTIFIER);
		}
	}

	public Notify fetch() throws WatchException {
		Notify notify = null;
		if (eventQue.isEmpty())
			read(fileDesc, eventQue);
		Wevent event = eventQue.poll(); 
		if (event == null)
			return null;
		String path = watches.get(event.getWatch());
		if (path == null)
			throw new WatchException(FATAL_ERROR);
		String file = event.getFile();
		if (file == null)
			throw new WatchException(FATAL_ERROR);
		String pathName = path + "/" + file;
		if (event.getType() == Wevent.CREATE) {
			System.out.println("CREATE");
			if (event.isDir())
				add(pathName, fileDesc, watches, pathes);
			notify = new Notify(Notify.CREATE, path, file, event.isDir());
		} else if (event.getType() == Wevent.DELETE) {
			System.out.println("DELETE");
			if (event.isDir()) {
				watches.remove(pathes.get(pathName));
				pathes.remove(pathName);
			}
			notify = new Notify(Notify.DELETE, path, file, event.isDir());
		} else if (event.getType() == Wevent.DELETE_SELF) {
			System.out.println("DELETE_SELF");
		} else if (event.getType() == Wevent.MODIFY) {
			System.out.println("MODIFY");
			notify = new Notify(Notify.MODIFY, path, file, event.isDir());
		} else if (event.getType() == Wevent.MOVED_FROM) {
			System.out.println("MOVED_FROM");
			if (event.isDir()) {
				clearWatch(pathName);
			}
			notify = new Notify(Notify.DELETE, path, file, event.isDir());
		} else if (event.getType() == Wevent.MOVED_TO) {
			System.out.println("MOVED_TO");
			if (event.isDir())
				add(pathName, fileDesc, watches, pathes);
			notify = new Notify(Notify.CREATE, path, file, event.isDir());
		}
		return notify;
	}

	private void clearWatch(String pathName) {
		Iterator<Map.Entry<String, Integer>> it = pathes.entrySet().iterator();
		while (it.hasNext()) {
			Map.Entry<String, Integer> e = it.next();
			String path = e.getKey();
			int watch = e.getValue();
			if (path.startsWith(pathName)) {
				watches.remove(watch);
				delete(fileDesc, watch);
				it.remove();
			}
		}
	}

	private native int init();

	private native boolean destory(int fileDesc);

	private native void read(int fileDesc, Queue<Wevent> eventQue);

	private native void add(String pathName, 
							int fileDesc, 
							Map<Integer, String> watches, 
							Map<String, Integer> pathes);
	
	private native void delete(int fileDesc, int watch);
	
}
