Asynchronous and event driven in lua
---

```lua
async.settimeout(2000, function() print("timeout 2000 --> this shall print last") end)
async.settimeout(1000, function() print("timeout 1000 --> this shall print second") end)
local handler = async.settimeout(3000, function() print("timeout 3000 --> this shall never call") end)
print("timeout 0    --> this shall print first")
async.cleartimeout(handler)
```

To build and test on MacOS

```bash
brew install lua
brew install libuv
git clone git@github.com:imwithye/lua_async.git && cd lua_async
make test
```