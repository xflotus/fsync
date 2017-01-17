package watch;

class Wevent {
	public static final int UNKNOWN = 0;
	public static final int CREATE = 1;
	public static final int DELETE = 2;
	public static final int DELETE_SELF = 3;
	public static final int MODIFY = 4;
	public static final int MOVED_FROM = 5;
	public static final int MOVED_TO = 6;

	private int watch;
	private String file;
	private int type;
	private int cookie;
	private boolean isDir;

	public int getWatch() {
		return watch;
	}

	public String getFile() {
		return file;
	}

	public int getType() {
		return type;
	}

	public int getCookie() {
		return cookie;
	}

	public boolean isDir() {
		return isDir;
	}
}
