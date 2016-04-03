			<#list messageContentVO.children as child>
			<#switch child.contentType>
				<#case typeDef.ATTRIBUTE>
				<#case typeDef.OBJECT>
			str += "${child.name} = " + ${child.name}.toString() + "\n";
				<#break>
				<#case typeDef.LIST>
			for (var ${child.name}Index:int = 0; ${child.name}Index < ${child.name}.length; ${child.name}Index++){
				str += "${child.name}[${child.name}Index] = " + ${child.name}[${child.name}Index].toString() + ", ";
			}
			str += "\n";
				<#break>
			</#switch>
			</#list>
