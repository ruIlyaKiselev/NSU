package Tetris.Controller;

import java.io.*;
import java.text.SimpleDateFormat;
import java.util.Calendar;

public class CsvWriter {
    public CsvWriter(String name, int record) throws IOException {

        try (PrintWriter writer = new PrintWriter(new FileWriter("records.csv", true))) {

            StringBuilder sb = new StringBuilder();
            sb.append(name);
            sb.append(" , ");
            sb.append(record);
            sb.append(" , ");
            sb.append(new SimpleDateFormat("yyyy.MM.dd HH:mm:ss").format(Calendar.getInstance().getTime()));
            sb.append('\n');

            writer.write(sb.toString());

        } catch (FileNotFoundException e) {
            throw e;
        } catch (IOException e) {
            throw e;
        }
    }
}
