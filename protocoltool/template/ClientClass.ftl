package com.cyou.webgame.ST.net.vo
{
	/**
	 * 文件名：${messageContentVO.type}.as
	 * <p>
	 * 功能：${messageContentVO.comment}
	 * <p>
	 * 版本：
	 * <p>
	 * 日期：
	 * <p>
	 * 作者：向金保
	 */
	public class ${messageContentVO.type}
	{
		<#list messageContentVO.children as child>
		/**${child.comment}*/
		<#switch child.contentType>
			<#case typeDef.ATTRIBUTE>
		public var ${child.name}:${child.type};
			<#break>
			<#case typeDef.OBJECT>
		public var ${child.name}:${child.type} = new ${child.type}();
			<#break>
			<#case typeDef.LIST>
		public var ${child.name}:Vector.<${child.type}> = new Vector.<${child.type}>();
			<#break>
		</#switch>
		</#list>
		
		public function toString():String{
			var str:String = "";
			<#include "/ClientClass/toString.ftl">
			return str;
		}
	}
}