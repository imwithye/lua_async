async.settimeout(2000, function() print("timeout 2000 --> this shall print last") end)
async.settimeout(1000, function() print("timeout 1000 --> this shall print second") end)
print("timeout 0    --> this shall print first")