using System;
using System.IO;
using System.Text;

class Program {
    static void Main() {
        var utf8 = new UTF8Encoding(false, true);
        string[] files = Directory.GetFiles(@"c:\Users\student\Desktop\AI\AI", "*.*");
        int count = 0;
        foreach(var f in files) {
            if (f.EndsWith(".cpp") || f.EndsWith(".h")) {
                byte[] bytes = File.ReadAllBytes(f);
                try {
                    utf8.GetString(bytes);
                    Console.WriteLine(Path.GetFileName(f) + " is VALID UTF-8");
                } catch {
                    Console.WriteLine(Path.GetFileName(f) + " is INVALID UTF-8");
                }
                count++;
            }
        }
        Console.WriteLine("Checked " + count + " files.");
    }
}
