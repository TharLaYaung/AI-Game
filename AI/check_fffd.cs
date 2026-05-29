using System;
using System.IO;

class Program {
    static void Main() {
        string[] files = Directory.GetFiles(@"c:\Users\student\Desktop\AI\AI", "*.*");
        foreach(var f in files) {
            if (f.EndsWith(".cpp") || f.EndsWith(".h")) {
                byte[] bytes = File.ReadAllBytes(f);
                for (int i = 0; i < bytes.Length - 2; i++) {
                    if (bytes[i] == 0xEF && bytes[i+1] == 0xBF && bytes[i+2] == 0xBD) {
                        Console.WriteLine(Path.GetFileName(f) + " contains U+FFFD at " + i.ToString("X"));
                        break;
                    }
                }
            }
        }
    }
}
