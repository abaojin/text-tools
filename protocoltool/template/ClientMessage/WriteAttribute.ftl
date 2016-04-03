<#assign var_tab_count = var_tab_count/>
<#switch var_attributeDefType>
<#case var_typeDef.NUMBER>
${""?left_pad(var_tab_count,"	")}this.writeDouble(${var_attributeName});
<#break>
<#case var_typeDef.INT>
${""?left_pad(var_tab_count,"	")}this.writeInt(${var_attributeName});
<#break>
<#case var_typeDef.SHORT>
${""?left_pad(var_tab_count,"	")}this.writeShort(${var_attributeName});
<#break>
<#case var_typeDef.STRING>
${""?left_pad(var_tab_count,"	")}this.writeString(${var_attributeName});
<#break>
<#case var_typeDef.FLOAT>
${""?left_pad(var_tab_count,"	")}this.writeFloat(${var_attributeName});
<#break>
<#case var_typeDef.BYTE>
${""?left_pad(var_tab_count,"	")}this.writeByte(${var_attributeName});
<#break>
<#case var_typeDef.LONG>
${""?left_pad(var_tab_count,"	")}this.writeLong(${var_attributeName});
<#break>
</#switch>
