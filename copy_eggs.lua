local function get_base_language_independent(addr, offset)
    return mainmemory.read_u32_le(mainmemory.read_u32_le(addr - 0x2000000) - 0x2000000 + offset)
end

local function get_binary_file_data(file)
    local data = nil
    local file = io.open(file, "rb")
    if file then
        data = file:read("*all")
        file:close()
    end
    return data
end

local function copy_eggs(bin_dir, base, categories)
    local size = 136
    for _, cat in ipairs(categories) do
        local file_count = #cat.files
        for i = 0, file_count-1 do
            local addr = base + cat.offset + i*size
            local file_data = get_binary_file_data(bin_dir..cat.files[i+1])
            if file_data then
                for j = 0, size-1 do
                    mainmemory.writebyte(addr + j - 0x2000000, string.byte(file_data, j+1))
                end
            end
        end
    end
end


local bin_dir = "out/"
local base = get_base_language_independent(0x2002848, -4)

local categories = {
    {
        category = "loader",
        offset = 0xc318,
        files = {"eggsloader_egg.bin"}
    },
    {
        category = "tetris",
        offset = 0xc3a0,
        files = {}
    },
    {
        category = "npc",
        offset = 0x1e060,
        files = {}
    }
}

for i = 0, 14 do
    local formatted_num = string.format("%02d", i)
    table.insert(categories[2].files, "tetris_egg"..formatted_num..".bin")
end

for i = 0, 1 do
    local formatted_num = string.format("%02d", i)
    table.insert(categories[3].files, "npcase_egg"..formatted_num..".bin")
end

copy_eggs(bin_dir, base, categories)


