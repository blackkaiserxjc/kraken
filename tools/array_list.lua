local M = {}

M.ArrayList = {}
M.ArrayList.__index = M.ArrayList

-- 创建
function M.ArrayList.new(...)
    local o = {}
    setmetatable(o, M.ArrayList)
    o:ctor(...)
    return o
end

-- 构造函数
function M.ArrayList:ctor(...)
    local array, capacity = table.unpack({...})
    if array ~= nil then
        self.array_ = array
        self.size_ = #array
        self.capacity_ = math.max(capacity, #array)
    else
        self:clear()
    end
end

-- 克隆函数
function M.ArrayList:clone()
    local temp = {}
    for key, value in pairs(self.array_) do
        temp[key] = value
    end
    return M.ArrayList.new(temp, self.capacity_)
end

-- set函数
function M.ArrayList:set(index, value)
    self.array_[index] = value
end

-- get操作
function M.ArrayList:get(index)
    return self.array_[index]
end

-- 大小
function M.ArrayList:size() 
    return self.size_
end

-- 是否空
function M.ArrayList:empty()
    return self.size_ == 0
end

-- 添加
function M.ArrayList:add(value)
    self.array_[self.size_] = value
    self.size_ = self.size_ + 1
    if self.size_ == self.capacity_ then
        self:resize(self.size_ * 2)
    end
end

-- 清空
function M.ArrayList:clear()
    self.array_ = {}
    self.size_ = 0
    self.capacity_ = 0
end

-- 是否包含
function M.ArrayList:contains(value)
    return self:indexof(value) ~= -1
end

-- 获取索引
function M.ArrayList:indexof(value)
    if self.size_ == 0 then
        return -1
    end
    for index = 0, self.size_ - 1 do
        if self.array_[index] == value then 
            return index
        end
    end
    return -1
end

-- 获取last index
function M.ArrayList:lastindexof(value)
    if self.size_ == 0 then
        return -1
    end
    for index = self.size_-1, 0, -1 do
        if self.array_[index] == value then
            return index 
        end
    end
    return -1 
end

-- 扩容
function M.ArrayList:resize(size)
    local array = {}
    for index = 0, size - 1 do 
        array[index] = self.array_[index]
    end
    self.array_ = array
    self.capacity_ = size
end

-- 删除
function M.ArrayList:remove(value)
    local index = self:lastindexof(value)
    if index ~= -1 then
        self:remove_at(index)
    end
end

-- 索引删除
function M.ArrayList:remove_at(index)
    if index == self.size_ - 1 then
        self.size_ = self.size_ - 1
        return 
    end
    
    for i = index + 1, self.size_ - 1 do
        self.array_[i - 1] = self.array_[i]
    end
    self.array_[self.size_ - 1] = nil
    self.size_ = self.size_ - 1
end

-- 转换为字符串
function M.ArrayList:to_string()
    local function _v(v)
        if type(v) == "string" then 
            v="\""..v.."\""
        end
        return tostring(v)
    end

    local result = ""
    for index = 0, self.size_ - 1 do 
        result = string.format( "%s%s : [%s]\n", result, index, _v(self.array_[index]))
    end
    return result
end

return M