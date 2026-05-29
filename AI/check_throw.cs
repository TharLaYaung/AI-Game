using System;
using System.IO;
using System.Text;

class Program {
    static void Main() {
        var utf8 = new UTF8Encoding(false, true);
        byte[] invalid = new byte[] { 0xFF, 0xFE };
        try {
            utf8.GetString(invalid);
            Console.WriteLine("Did not throw!");
        } catch {
            Console.WriteLine("Threw!");
        }
    }
}
