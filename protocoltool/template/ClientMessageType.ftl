package com.cyou.webgame.ST.net{
	/**
	 * 文件名：HTTPMessageType.as
	 * <p>
	 * 功能：客户端HTTP通信消息号常量
	 * <p>
	 * 版本：
	 * <p>
	 * 日期：
	 * <p>
	 * 作者：向金保
	 */
	public class HTTPMessageType{

<#list messages as message>
        /**
		 * ${message.comment}
		 */		
		public static const ${message.type}:int = ${message.id?string.computer};
</#list>
	}
}