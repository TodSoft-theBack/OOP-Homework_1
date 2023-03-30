	MarkdownTableParser

Commands:

    - Load <File name>

    - Save <File name>

    - Add
	* Add Row <array of values seperated with spaces> (ex. Add Row Header HTML Component)
        * Add Column (Not yet implemented)

    - Change
        * Change ColumnName <Current column name> <New column name> (ex. Change ColumnName Syntax Component)
        * Change Value <Row index> <Column name> <New value> (ex. Change Value 3 Syntax Footer)
	!!! <Row index> is 1-based and refers to just the data in the table !!!
	!!! The headers row is accessed with the previous command !!!
	!!! To change the formating row you can use: Change Value 1 <Column name> <New value> !!!
        * Change Value <Column name> <Current value> <New value> (ex. Change Value Syntax Syntax Footer)
	!!! In case <Current value> appears multiple times in the given column only the first occurrance is changed !!!

    - Select
        * Select <Column name> <Value>

	TextFilesManager

Commands:
