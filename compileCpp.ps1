$files = Get-ChildItem -Recurse -Path .\rewrite-client -Filter *.cpp | ForEach-Object { $_.FullName }
g++ -o client.exe $files -lws2_32
