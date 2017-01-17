package watch;

public class Notify {

	public static final int UNKNOWN = 0;
	public static final int CREATE = 1;
	public static final int DELETE = 2;
	public static final int MODIFY = 3;

	private int type;
	private String path;
	private String file;
	private boolean isDir;

	Notify(int type, String path, String file, boolean isDir) {
		this.type = type;
		this.path = path;
		this.file = file;
		this.isDir = isDir;
	}

	public int getType() {
		return type;
	}

	public String getPath() {
		return path;
	}

	public String getFile() {
		return file;
	}

	public boolean isDir() {
		return isDir;
	}

	public String toString() {
		String str = "----------------------------\n";
		str += "File Name: " + path + "/" + file + "\n";
		str += "Type: ";
		switch (type) {
		case CREATE:
			str += "CREATE\n";
			break;
		case DELETE:
			str += "DELETE\n";
			break;
		case MODIFY:
			str += "MODIFY\n";
			break;
		default:
			str += "UNKNOWN\n";
			break;
		}
		str += "Is directory: " + (isDir() ? "true\n" : "false\n");
		return str;
	}
}
