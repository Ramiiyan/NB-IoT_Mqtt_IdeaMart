<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:import href="main.xslt" />
    <xsl:template match="/">
        <clients>
            <xsl:for-each select="//people/person">
                <xsl:call-template name="FILL_PPL"></xsl:call-template>
            </xsl:for-each>
        </clients>
    </xsl:template>
</xsl:stylesheet>