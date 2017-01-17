package watch;

import java.util.*;

public class WatchException extends Exception {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	private int errcode;

	public static final int INVALID_PATHNAME = 0;
	public static final int FAIL_TO_CREATE_INOTIFIER = 1;
	public static final int FAIL_TO_RELEASE_INOTIFIER = 2;
	public static final int FATAL_ERROR = 3;

	public WatchException(int errcode) {
		this.errcode = errcode;
	}

	public int getErrCode() {
		return errcode;
	}
	
	public String toString() {
		switch (errcode) {
		case INVALID_PATHNAME: 
			return "Path name does not exist or is not directory."; 
		case FAIL_TO_CREATE_INOTIFIER:
			return "Fail to initialize an inotify instance.";
		case FAIL_TO_RELEASE_INOTIFIER:
			return "Fail to release an inotify instance.";
		case FATAL_ERROR:
			return "Encounter an error which can not be fixed.";
		}
		return "";
	}
}
