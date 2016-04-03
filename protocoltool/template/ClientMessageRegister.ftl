package com.cyou.webgame.ST.net{
	import com.cyou.webgame.ST.core.net.HTTPLoaderManager;
	import com.cyou.webgame.ST.net.custom.*;
	/**
	 * 文件名：HTTPMessageRegister.as
	 * <p>
	 * 功能：客户端HTTP通信消息注册类
	 * <p>
	 * 版本：
	 * <p>
	 * 日期：
	 * <p>
	 * 作者：向金保
	 */
	public class HTTPMessageRegister{

		private static var _instance:HTTPMessageRegister;

		public static function getInstance():HTTPMessageRegister{
			if(_instance == null){
				_instance = new HTTPMessageRegister();
			}
			return _instance;
		}
	
		/**
		 * 初始化注册
		 */	
		public function init():void{
<#list messages as message>
			//${message.comment}
			this.addMessage(HTTPMessageType.${message.type}, ${message.name});
</#list>		
		}

		/**
		 * 注册到一个统一的集合中
		 */	
		private function addMessage(messageKey:int,messageClass:Class):void{
			HTTPLoaderManager.getInstance().addMessageClass(messageKey, messageClass);
		}
	}
}