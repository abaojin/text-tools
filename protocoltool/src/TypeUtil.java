public class TypeUtil {

	

	public static String parseDefTypeToRealType(String defType) {
		String str = "";
		String def = defType;
		TypeDef td = new TypeDef();
		if (def.equals(td.NUMBER)) {
			str = "Number";
		} else if (def.equals(td.INT)) {
			str = "int";
		} else if (def.equals(td.SHORT)) {
			str = "int";
		} else if (def.equals(td.STRING)) {
			str = "String";
		} else if (def.equals(td.FLOAT)) {
			str = "Number";
		} else if (def.equals(td.BYTE)) {
			str = "int";
		} else if (def.equals(td.LONG)) {
			str = "String";
		}
		if(str == ""){
			try {
				throw new Exception("�������������: " + defType);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return str;
	}
	
	public static String parseDefTypeToRealServerType(String defType){
		String str = "";
		String def = defType;
		TypeDef td = new TypeDef();
		if (def.equals(td.NUMBER)) {
			str = "Number";
		} else if (def.equals(td.INT)) {
			str = "int";
		} else if (def.equals(td.SHORT)) {
			str = "short";
		} else if (def.equals(td.STRING)) {
			str = "char";
		} else if (def.equals(td.FLOAT)) {
			str = "float";
		} else if (def.equals(td.BYTE)) {
			str = "byte";
		} else if (def.equals(td.LONG)) {
			str = "long";
		}
		if(str == ""){
			try {
				throw new Exception("�������������: " + defType);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return str;
	}
	
	public static String parseDefTypeToJavaServerType(String defType)
	{
		String str = "";
		String def = defType;
		TypeDef td = new TypeDef();
		if (def.equals(td.NUMBER)) {
			str = "Number";
		} else if (def.equals(td.INT)) {
			str = "int";
		} else if (def.equals(td.SHORT)) {
			str = "short";
		} else if (def.equals(td.STRING)) {
			str = "String";
		} else if (def.equals(td.FLOAT)) {
			str = "float";
		} else if (def.equals(td.BYTE)) {
			str = "byte";
		} else if (def.equals(td.LONG)) {
			str = "long";
		}
		if(str == ""){
			try {
				throw new Exception("�������������: " + defType);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return str;
	}

}
