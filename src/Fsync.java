import watch.*;
import java.util.*;

public class Fsync {
	public static void main(String[] args) {
		System.out.print("Input a path to be monited: ");
		Scanner input = new Scanner(System.in);
		String base = input.nextLine();
		Watch fl = new LinuxWatch(base);
		try {
			fl.start();
			while (true) {
				Notify n = fl.fetch();
				if (n != null)
					System.out.println(n);
			}
			// fl.stop();
		} catch (WatchException e) {
			System.out.println(e);
		}
	}
}
