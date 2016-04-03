package com.cyou.wg.spriteTales.protocol.vo;
import com.cyou.wg.sns.gs.core.domain.BaseObject;
import org.apache.mina.core.buffer.IoBuffer;
	/**
	 * 文件名：${messageContentVO.type}.as
	 * <p>
	 * 功能：${messageContentVO.comment}
	 */
public class ${messageContentVO.type}
{
	<#--要输出的基本属性-->
	<#list messageContentVO.children as child>
	<#switch child.contentType>
		<#case typeDef.ATTRIBUTE>
	public ${child.javaType} ${child.name};
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
	public ${child.type} ${child.name};
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
	public ${child.type}[] ${child.name} = new ${child.type}[0];
	/**${child.comment}*/
	public void set${child.name?cap_first}(${child.type}[] ${child.name}){
		this.${child.name} = ${child.name};
	}
	/**${child.comment}*/
	public ${child.type}[] get${child.name?cap_first}(){
		return ${child.name};
	}

		<#else>
	public ${child.javaType}[] ${child.name} = new ${child.javaType}[0];
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
	
	
	public BaseObject decode(IoBuffer src) throws Exception {
		// TODO Auto-generated method stub
		return null;
	}
	public byte[] encode() throws Exception {
		// TODO Auto-generated method stub
		return null;
	}
	
}
