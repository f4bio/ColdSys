function WriteKeyValueUpper(section, name, value, file)
	SKIN:Bang('!WriteKeyValue ' .. section .. ' ' .. name .. ' "' .. string.upper(value) .. '" "' .. file .. '"')
end