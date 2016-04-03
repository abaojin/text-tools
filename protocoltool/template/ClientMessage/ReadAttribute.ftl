<#assign var_tab_count = var_tab_count/>
<#switch var_attributeDefType>
<#case var_typeDef.NUMBER>
${""?left_pad(var_tab_count,"	")}${var_attributeName} = this.readDouble();
<#break>
<#case var_typeDef.INT>
${""?left_pad(var_tab_count,"	")}${var_attributeName} = this.readInt();
<#break>
<#case var_typeDef.SHORT>
${""?left_pad(var_tab_count,"	")}${var_attributeName} = this.readShort();
<#break>
<#case var_typeDef.STRING>
${""?left_pad(var_tab_count,"	")}${var_attributeName} = this.readString();
<#break>
<#case var_typeDef.FLOAT>
${""?left_pad(var_tab_count,"	")}${var_attributeName} = this.readFloat();
<#break>
<#case var_typeDef.BYTE>
${""?left_pad(var_tab_count,"	")}${var_attributeName} = this.readByte();
<#break>
<#case var_typeDef.LONG>
${""?left_pad(var_tab_count,"	")}${var_attributeName} = this.readLong();
<#break>
</#switch>
