<#if messageVO.messageTypeID == 1>package com.cyou.wg.spriteTales.protocol.request;<#else>package com.cyou.wg.spriteTales.protocol.response;</#if>
import java.io.IOException;
import org.apache.mina.core.buffer.IoBuffer;
import com.cyou.wg.sns.gs.core.cache.ThreadLocalCache;
import com.cyou.wg.sns.gs.core.exception.CyouSysException;
import com.cyou.wg.sns.gs.core.protocol.RequestProtocol;
import com.cyou.wg.sns.gs.core.protocol.ResponseProtocol;
import com.cyou.wg.sns.gs.core.protocol.base.response.FailResponse;
import com.cyou.wg.sns.gs.core.util.ByteUtil;
import com.cyou.wg.spriteTales.protocol.vo.*;
/**
	 * 文件名：${messageVO.name}.java
	 * <p>
	 * 功能：${messageVO.comment}
	 */
public class ${messageVO.name} extends <#if messageVO.messageTypeID == 1>RequestProtocol<#else>ResponseProtocol</#if>{
	private final static String handler = "${messageVO.handler}Handler";

	<#--要输出的基本属性-->
	<#list messageVO.children as child>
	<#switch child.contentType>
		<#case typeDef.ATTRIBUTE>
	private ${child.javaType} ${child.name};
	/**${child.comment}*/
	public void set${child.name?cap_first}(${child.javaType} ${child.name}){
		this.${child.name} = ${child.name};
	}
	/**${child.comment}*/
	public ${child.javaType} get${child.name?cap_first}(){
		return ${child.name};
	}

		<#break>
		<#case typeDef.OBJECT>
	private ${child.type} ${child.name};
	/**${child.comment}*/
	public void set${child.name?cap_first}(${child.type} ${child.name}){
		this.${child.name} = ${child.name};
	}
	/**${child.comment}*/
	public ${child.type} get${child.name?cap_first}(){
		return ${child.name};
	}

		<#break>
		<#case typeDef.LIST>
		<#if child.hasChildren>
	private ${child.type}[] ${child.name} = new ${child.type}[0];
	/**${child.comment}*/
	public void set${child.name?cap_first}(${child.type}[] ${child.name}){
		this.${child.name} = ${child.name};
	}
	/**${child.comment}*/
	public ${child.type}[] get${child.name?cap_first}(){
		return ${child.name};
	}

		<#else>
	private ${child.javaType}[] ${child.name} = new ${child.javaType}[0];
	/**${child.comment}*/
	public void set${child.name?cap_first}(${child.javaType}[] ${child.name}){
		this.${child.name} = ${child.name};
	}
	/**${child.comment}*/
	public ${child.javaType}[] get${child.name?cap_first}(){
		return ${child.name};
	}
		</#if>
		<#break>
	</#switch>
	</#list>

	<#if messageVO.messageTypeID == 1>
	@Override
	public void decode(IoBuffer src) {
		try {
			/* 读取数据 */
			<#assign var_tab_count = 3/>
			<#assign var_children = messageVO.children/>
			<#assign var_attributeNameTitle = 'this'/>
			<#assign var_listNameTitle = 'this'/>
			<#assign var_lastListName = ""/>
			<#assign var_lastListCurrentName = ""/>
			<#include "/ServerMessage/ReadByteArray.ftl">		
			
		} catch (Exception e) {
			e.printStackTrace();
			throw new CyouSysException("请求协议：" + super.getProtocolId() + "解码错误");
		}
	}

	</#if>
	public short getProtocolId() {
	    return ${messageVO.id?string.computer};
	}
	<#if messageVO.messageTypeID == 2>
	@Override
	public byte[] encode() throws Exception {
		IoBuffer src = IoBuffer.allocate(64);
		src.setAutoExpand(true);
		src.putShort(getProtocolId());
		/* 写入数据 */
		<#assign var_tab_count = 3/>
		<#assign var_children = messageVO.children/>
		<#assign var_attributeNameTitle = 'this'/>
		<#assign var_listNameTitle = 'this'/>
		<#assign var_listName = 'this'/>
		<#include "/ServerMessage/WriteByteArray.ftl">

		return ByteUtil.buff2Array(src);
	}

	</#if>
	<#if messageVO.messageTypeID == 1>
	public ResponseProtocol execute() {
		return super.execute(this,handler);
	}
	</#if>

}