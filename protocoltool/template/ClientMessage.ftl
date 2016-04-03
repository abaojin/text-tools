package com.cyou.webgame.ST.net.custom
{
	import flash.utils.ByteArray;
	import com.cyou.webgame.ST.net.vo.*;
	import com.cyou.webgame.ST.net.*;
	import com.cyou.webgame.ST.core.net.*;
	/**
	 * 文件名：${messageVO.name}.as
	 * <p>
	 * 功能：${messageVO.comment}
	 * <p>
	 * 版本：
	 * <p>
	 * 日期：
	 * <p>
	 * 作者：向金保
	 */
	public class ${messageVO.name} extends <#if messageVO.messageTypeID == 1>MessageEncodeBase<#else>MessageDecodeBase</#if>
	{
		<#--要输出的基本属性-->
		<#list messageVO.children as child>
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


		override public function get byteArray():ByteArray
		{
		<#--如果是一个返回消息，那么不能获取byteArray-->
		<#if messageVO.messageTypeID == 1>
			/* 给头信息留出空位 */
			_byteArray.length = 2;
			_byteArray.position = 2;
			
			/* 写入数据 */
			<#assign var_tab_count = 3/>
			<#assign var_children = messageVO.children/>
			<#assign var_attributeNameTitle = 'this'/>
			<#assign var_listNameTitle = 'this'/>
			<#assign var_listName = 'this'/>
			<#include "/ClientMessage/WriteByteArray.ftl">

			//整理消息的字节数组   加入消息号,消息类型
			this.packageByteArray();
			return this._byteArray;
		<#else>
			throw new Error("${messageVO.name}.byteArray(getter)");
			return null;
		</#if>
		}

		override public function set byteArray(value:ByteArray):void
		{
		<#--如果是一个发送消息，那么不能注入byteArray-->
		<#if messageVO.messageTypeID == 1>
			throw new Error("${messageVO.name}.byteArray(setter)");
		<#else>
			/* 判断长度 */
			if(value.length < 4) return;
			this._byteArray = value;
			//整理消息的基本信息   消息号,消息类型
			this.packageMessageInfo();
			
			/* 读取数据 */
			<#assign var_tab_count = 3/>
			<#assign var_children = messageVO.children/>
			<#assign var_attributeNameTitle = 'this'/>
			<#assign var_listNameTitle = ''/>
			<#assign var_listName = ''/>
			<#assign var_lastListName = ""/>
			<#assign var_lastListCurrentName = ""/>
			<#include "/ClientMessage/ReadByteArray.ftl">

			
		</#if>
		}
		
		
		public function ${messageVO.name}()
		{
			this._messageID = HTTPMessageType.${messageVO.type};
			this._messageType = "${messageVO.type}";
		}
				
		
		override public function toString():String {
			var str:String = "";
			<#list messageVO.children as child>
			<#switch child.contentType>
				<#case typeDef.ATTRIBUTE>
				<#case typeDef.OBJECT>
			str += "${child.name} = " + ${child.name}.toString() + "\n";
				<#break>
				<#case typeDef.LIST>
			
			for (var ${child.name}Index:int = 0; ${child.name}Index < ${child.name}.length; ${child.name}Index++){
				str += "${child.name}[" + ${child.name}Index + "] = " + ${child.name}[${child.name}Index].toString() + ", ";
			}
			str += "\n";
				<#break>
			</#switch>
			</#list>
			return super.toString() + str;
		}
		/**
		 * 清除Message中字节数组的信息
		 */
		public function clear():void {
			_byteArray.length = 0;
		}
	}
}
