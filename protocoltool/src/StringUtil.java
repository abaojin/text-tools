public class StringUtil {

	public static String toUpperCaseFirstChart(String source) {
		return Character.toUpperCase(source.charAt(0)) + source.substring(1);
	}
}
