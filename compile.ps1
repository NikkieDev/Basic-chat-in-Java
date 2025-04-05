# Get all .java files recursively and pass them to javac
$sources = Get-ChildItem -Recurse -Filter *.java
javac -d dist $sources.FullName
