package com.example.kalpesh.camera;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
//import android.util.Config;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

//import java.awt.*;

import java.io.*;
//import java.util.Scanner;

//import javax.swing.JFrame;
import java.io.File;
import java.io.IOException;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.Date;

import static android.widget.ImageView.*;



public class MainActivity extends ActionBarActivity {
    static final int REQUEST_TAKE_PHOTO = 1;
    String messagefinal = "";
    int stickercount = 0;
    Bitmap image;
    Bitmap test;
    Bitmap test2;
    int width;
    int height;
    double possibleLengths[][]=new double[2][3];
    static int red=0;
    static int blue=0;
    static int green=0;
    final int noise_removal=0;
    final int color_detection=1;
    int n1=0,n2=0;
    String[]colorname={"blue","red","green","yellow"};
    int counter=0;

    int color=0;
    //To be decided
    double stddeviationcap=3;
    int mincount=100;
    double minlength=10;
    int interval=1;
    //To be decided
    int limit=60000;
    double lowertriangles[][]=new double[100][4];
    double uppertriangles[][]=new double[100][4];
    int lowertrianglecount=0;
    int uppertrianglecount=0;
    int blobTopX=0;
    int blobTopY=1500;
    int blobLeftX=1500;
    int blobLeftY=0;
    int blobRightX=0;
    int blobRightY=0;
    int blobBottomX=0;
    int blobBottomY=0;
    double colors[] = new double[9];
    int frontier[][]=new int[10000][2];
    int prevfrontier[][]=new int[10000][2];
    int frontiercount=0;
    int prevfrontiercount=0;
    //boolean stickerstatus=false;
    int colorcode[][]={{0,0,255},{255,0,0},{0,255,0},{255,255,0}};
    int[][]surround={{1,1,1,0,0,-1,-1,-1},{1,0,-1,1,-1,1,0,-1}};
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    String mCurrentPhotoPath;

    public void sendData(View view) {

        ConnectivityManager connMgr = (ConnectivityManager)
                getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = connMgr.getActiveNetworkInfo();
        if (networkInfo != null && networkInfo.isConnected()) {
            new sendDataASync().execute(messagefinal);

        } else {
            // display error
        }
        TextView t=(TextView)findViewById(R.id.edit_message);
        t.setText("Successfully Sent!!");

    }

    static final int REQUEST_IMAGE_CAPTURE = 1;

    public void clickPic(View view) {
        for(int i=0;i<9;i++) colors[i]=0;
        lowertrianglecount=0;
        uppertrianglecount=0;
        for (int i=0;i<100;i++)
        {
            for (int j=0;j<4;j++)
            {
                lowertriangles[i][j]=0;
                uppertriangles[i][j]=0;
            }
        }
        Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        if (takePictureIntent.resolveActivity(getPackageManager()) != null) {
            startActivityForResult(takePictureIntent, REQUEST_IMAGE_CAPTURE);
        }
    }


    private void galleryAddPic() {
        Intent mediaScanIntent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
        File f = new File(mCurrentPhotoPath);
        Uri contentUri = Uri.fromFile(f);
        mediaScanIntent.setData(contentUri);
        this.sendBroadcast(mediaScanIntent);
    }





    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_IMAGE_CAPTURE && resultCode == RESULT_OK) {
            Bundle extras = data.getExtras();

            Bitmap image2 = (Bitmap) extras.get("data");
            //image = Bitmap.createBitmap(image2.getWidth(), image2.getHeight(), Bitmap.Config.ARGB);
            //
            image = image2.copy(image2.getConfig(), true);
            test = image2.copy(image2.getConfig(), true);
            test2 = image2.copy(image2.getConfig(), true);
            TextView smallText = (TextView) findViewById(R.id.edit_message);
            width = image.getWidth();
            height = image.getHeight();
            //Deciding parameters
            stddeviationcap=3*((double)width/90)*((double)height/162);;
            double mincount1=100*((double)(width*height)/(90*162));
            mincount=(int)mincount1;
            minlength=10*((double)width/90)*((double)height/162);
            Log.d("Values of stddev, mincount,min length",stddeviationcap+","+mincount+","+minlength);
            //Deciding parameters
            for (int i=0;i<width;i++)
            {
                for (int j=0;j<height;j++)
                {
                    test.setPixel(i, j, Color.WHITE);
                    test2.setPixel(i, j, Color.WHITE);
                }
            }
            for (int i=0;i<width;i++)
            {
                for (int j=0;j<height;j++)
                {
                    //Log.e("MainActivity","("+i+","+j+")"+" is ("+Color.red(image.getPixel(i,j))+","+Color.green(image.getPixel(i,j))+","+Color.blue(image.getPixel(i,j))+")");
                }
            }
            messagefinal = "";
            stickercount = 0;
            color = 0;

            //Log.e("MainActivity",width+" "+height);
            findColors();

            noiseRemoval();
            ImageView mImageView = (ImageView) findViewById(R.id.mImageView);
            Bitmap resized = Bitmap.createScaledBitmap(test2, (int)width*3, (int)height*3, true);
            mImageView.setImageBitmap(resized);
            smallText.setText(messagefinal);
            //Log.e("MainActivity",messagefinal);

        }
    }

    boolean colorTest(int cd, int k,int mode) {
        if (mode == noise_removal) {

            if (k == 0) {
                if (cd==Color.BLUE)
                    return true;
                else return false;
            } else if (k == 1) {
                if (cd==Color.RED)
                    return true;
                else return false;
            } else if (k == 2) {
                if (cd==Color.GREEN)
                    return true;
                else return false;
            } else if (k == 3) {
                if (cd==Color.YELLOW)
                    return true;
                else return false;
            }
            return false;
        } else {
            float hsb[] = new float[3];
            Color.RGBToHSV(Color.red(cd), Color.green(cd), Color.blue(cd), hsb);
            //Log.d("HSB values at ",hsb[0]+","+hsb[1]+","+hsb[2]);
            hsb[0] = 1 - hsb[0] / 360;
            if (k == 0) {
                if (hsb[0] > 0.40 && hsb[0] < 0.47 && hsb[1] > 0.5 && hsb[2] > 0.6)
                    return true;
                else return false;
            } else if (k == 1) {
                if ((hsb[0] < 0.05 || hsb[0] > 0.98) && hsb[1] > 0.5 && hsb[2] > 0.4)
                    return true;
                else return false;
            } else if (k == 2) {
                if (hsb[0] > 0.56 && hsb[0] < 0.61 && hsb[1] > 0.5 && hsb[2] > 0.4)
                    return true;
                else return false;
            } else if (k == 3) {
                if (hsb[0] > 0.7 && hsb[0] < 0.82 && hsb[1] > 0.2 && hsb[2] > 0.4)
                    return true;
                else return false;
            }
            return false;
        }
    }
    void noiseRemoval() {
        color = 0;
        for (int k = 0; k < 4; k++) {
            for (int i = 0; i < width; i += interval) {
                for (int j = 0; j < height; j += interval) {
                    counter = 0;
                    int rg = test.getPixel(i,j);
                    if (colorTest(rg, k,noise_removal)) {
                        //Log.d("Color detected");
                        blobTopY = 1500;
                        blobTopX = 0;
                        blobLeftX = 1500;
                        blobLeftY = 0;
                        blobRightX = 0;
                        blobRightY = 0;
                        blobBottomX = 0;
                        blobBottomY = 0;
                        //System.out.println("phase");
                        manip2(i, j);
                        //System.out.println(i+","+j+","+counter);

                        //System.out.println(counter);


                        //System.out.println("abc");
                  /*while (counter==limit)
                  {
                     counter=0;
                     manip(n1,n2);
                  }*/
                        if (counter > mincount) {
                            drawLine(blobBottomX, blobBottomY, blobLeftX, blobLeftY);
                            drawLine(blobBottomX, blobBottomY, blobRightX, blobRightY);
                            drawLine(blobTopX, blobTopY, blobLeftX, blobLeftY);
                            drawLine(blobTopX, blobTopY, blobRightX, blobRightY);
                            drawLine(blobRightX, blobRightY, blobLeftX, blobLeftY);
                            possibleLengths[0][0] = distance(blobRightX, blobRightY, blobLeftX, blobLeftY);
                            possibleLengths[0][1] = distance(blobTopX, blobTopY, blobLeftX, blobLeftY);
                            possibleLengths[0][2] = distance(blobTopX, blobTopY, blobRightX, blobRightY);
                            possibleLengths[1][0] = distance(blobRightX, blobRightY, blobLeftX, blobLeftY);
                            possibleLengths[1][1] = distance(blobBottomX, blobBottomY, blobLeftX, blobLeftY);
                            possibleLengths[1][2] = distance(blobBottomX, blobBottomY, blobRightX, blobRightY);
                            double stddev[] = new double[2];
                            for (int a = 0; a < 2; a++) {
                                double avg = 0;
                                for (int b = 0; b < 3; b++) {
                                    avg += possibleLengths[a][b];
                                    //Log.e("MainActivity",""+(double)Math.round(possibleLengths[a][b]*100)/100+" ");
                                }
                                avg = avg / 3;
                                stddev[a] = 0;
                                for (int b = 0; b < 3; b++)
                                    stddev[a] += Math.pow(avg - possibleLengths[a][b], 2);
                                stddev[a] = Math.sqrt(stddev[a] / 3);
                                //System.out.println(stddev[1]);
                                Log.e("MainActivity",stddev[0]+","+stddev[1]+",");
                            }

                            if (stddev[0] < stddeviationcap && possibleLengths[0][0] > minlength) {
                                //System.out.println("Upper Triangle at (" + i + "," + j + ")");
                                uppertriangles[uppertrianglecount][0] = i;
                                uppertriangles[uppertrianglecount][1] = j;
                                uppertriangles[uppertrianglecount][2] = possibleLengths[0][0];
                                uppertriangles[uppertrianglecount][3] = k;
                                uppertrianglecount++;
                            }
                            if (stddev[1] < stddeviationcap && possibleLengths[1][0] > minlength) {
                                //System.out.println("Lower Triangle at (" + i + "," + j + ")");
                                lowertriangles[lowertrianglecount][0] = i;
                                lowertriangles[lowertrianglecount][1] = j;
                                lowertriangles[lowertrianglecount][2] = possibleLengths[0][0];
                                lowertriangles[lowertrianglecount][3] = k;
                                lowertrianglecount++;
                            }


                        }
                    }
                }
            }
            color++;
        }
        if (uppertrianglecount > 6) {
            for (int i = 0; i < uppertrianglecount - 1; i++) {
                for (int j = i + 1; j < uppertrianglecount; j++) {
                    if (uppertriangles[i][2] <= uppertriangles[j][2]) {
                        for (int k = 0; k < 4; k++) {
                            double temp = uppertriangles[i][k];
                            uppertriangles[i][k] = uppertriangles[j][k];
                            uppertriangles[j][k] = temp;
                        }
                    }
                }
            }
        }
        if (lowertrianglecount > 3) {
            for (int i = 0; i < lowertrianglecount - 1; i++) {
                for (int j = i + 1; j < lowertrianglecount; j++) {
                    if (lowertriangles[i][2] <= lowertriangles[j][2]) {
                        for (int k = 0; k < 4; k++) {
                            double temp = lowertriangles[i][k];
                            lowertriangles[i][k] = lowertriangles[j][k];
                            lowertriangles[j][k] = temp;
                        }
                    }
                }
            }
        }
        lowertrianglecount = 3;
        uppertrianglecount = 6;

        for (int i = 0; i < lowertrianglecount - 1; i++) {
            for (int j = i + 1; j < lowertrianglecount; j++) {
                if (lowertriangles[i][1] > lowertriangles[j][1]) {
                    for (int k = 0; k < 4; k++) {
                        double temp = lowertriangles[i][k];
                        lowertriangles[i][k] = lowertriangles[j][k];
                        lowertriangles[j][k] = temp;
                    }
                }
            }
        }
        if (lowertriangles[1][0] > lowertriangles[2][0]) {
            for (int k = 0; k < 4; k++) {
                double temp = lowertriangles[1][k];
                lowertriangles[1][k] = lowertriangles[2][k];
                lowertriangles[2][k] = temp;
            }
        }
        for (int i = 0; i < uppertrianglecount - 1; i++) {
            for (int j = i + 1; j < uppertrianglecount; j++) {
                if (uppertriangles[i][1] > uppertriangles[j][1]) {
                    for (int k = 0; k < 4; k++) {
                        double temp = uppertriangles[i][k];
                        uppertriangles[i][k] = uppertriangles[j][k];
                        uppertriangles[j][k] = temp;
                    }
                }
            }
        }
        if (uppertriangles[1][0] > uppertriangles[2][0]) {
            for (int k = 0; k < 4; k++) {
                double temp = uppertriangles[1][k];
                uppertriangles[1][k] = uppertriangles[2][k];
                uppertriangles[2][k] = temp;
            }
        }
        for (int i = 3; i < uppertrianglecount - 1; i++) {
            for (int j = i + 1; j < uppertrianglecount; j++) {
                if (uppertriangles[i][0] > uppertriangles[j][0]) {
                    for (int k = 0; k < 4; k++) {
                        double temp = uppertriangles[i][k];
                        uppertriangles[i][k] = uppertriangles[j][k];
                        uppertriangles[j][k] = temp;
                    }
                }
            }
        }

        colors[0] = uppertriangles[0][3];
        colors[1] = uppertriangles[1][3];
        colors[2] = lowertriangles[0][3];
        colors[3] = uppertriangles[2][3];
        colors[4] = uppertriangles[3][3];
        colors[5] = lowertriangles[1][3];
        colors[6] = uppertriangles[4][3];
        colors[7] = lowertriangles[2][3];
        colors[8] = uppertriangles[5][3];
        for (int i=0;i<9;i++) messagefinal+=""+(int)colors[i];
        Log.d("final message",messagefinal);
    }
    void manip2(int i,int j)
    {
        if (j<blobTopY)
        {
            blobTopX=i;
            blobTopY=j;
        }
        if (i<blobLeftX)
        {
            blobLeftX=i;
            blobLeftY=j;
        }
        if (i>blobRightX)
        {
            blobRightX=i;
            blobRightY=j;
        }
        if (j>blobBottomY)
        {
            blobBottomX=i;
            blobBottomY=j;
        }
        prevfrontier[0][0]=i;
        prevfrontier[0][1]=j;
        counter=1;
        prevfrontiercount++;

        test.setPixel(i,j,Color.WHITE);
        while (prevfrontiercount!=0)
        {
            frontiercount=0;

            for (int l=0;l<prevfrontiercount;l++)
            {
                for (int k=0;k<8;k++)
                {
                    if (prevfrontier[l][0]+surround[0][k]>=0 && prevfrontier[l][0]+surround[0][k]<width && prevfrontier[l][1]+surround[1][k]>=0 && prevfrontier[l][1]+surround[1][k]<height)
                    {
                        //System.out.println(k);

                        int abc=test.getPixel(prevfrontier[l][0]+surround[0][k],prevfrontier[l][1]+surround[1][k]);
                        if (colorTest(abc,color,noise_removal))
                        {
                            frontier[frontiercount][0]=prevfrontier[l][0]+surround[0][k];
                            frontier[frontiercount][1]=prevfrontier[l][1]+surround[1][k];
                            counter++;
                            if (frontier[frontiercount][1]<blobTopY)
                            {
                                blobTopX=frontier[frontiercount][0];
                                blobTopY=frontier[frontiercount][1];
                            }
                            if (frontier[frontiercount][0]<blobLeftX)
                            {
                                blobLeftX=frontier[frontiercount][0];
                                blobLeftY=frontier[frontiercount][1];
                            }
                            if (frontier[frontiercount][0]>blobRightX)
                            {
                                blobRightX=frontier[frontiercount][0];
                                blobRightY=frontier[frontiercount][1];
                            }
                            if (frontier[frontiercount][1]>blobBottomY)
                            {
                                blobBottomX=frontier[frontiercount][0];
                                blobBottomY=frontier[frontiercount][1];
                            }
                            //System.out.println((i+surround[0][k])+","+(j+surround[1][k]));
                            //manip(i+surround[0][k],j+surround[1][k]);
                            test.setPixel(frontier[frontiercount][0],frontier[frontiercount][1],Color.WHITE);
                            frontiercount++;
                        }
                    }
                }
            }
            prevfrontiercount=frontiercount;
            for (int k=0;k<frontiercount;k++) prevfrontier[k][0]=frontier[k][0];
            for (int k=0;k<frontiercount;k++) prevfrontier[k][1]=frontier[k][1];
            prevfrontiercount=frontiercount;
        }
    }
    double distance(int startx,int starty,int endx,int endy)
    {
        double d=Math.sqrt((endx-startx)*(endx-startx)+(endy-starty)*(endy-starty));
        return d;
    }
    void drawLine(int startx,int starty,int endx,int endy)
    {
        try {
            if (endx<startx)
            {
                int temp1=endx;
                endx=startx;
                startx=temp1;
                temp1=endy;
                endy=starty;
                starty=temp1;
            }
            double slope = ((double)endy-starty)/(endx-startx);
            //Color BLACK=new Color(0,0,0);
            int prevy=0;
            for (int i=0;i<=(endx-startx);i++)
            {
                double y=slope*i;
                int j=prevy;
                while ((j<=Math.round(y) && slope>=0) || (j>=Math.round(y) && slope<0) )
                {
                    //System.out.println(i+","+j);
                    test2.setPixel(startx+i,starty+j,Color.BLACK);
                    if (slope>=0)
                        j++;
                    else j--;
                }
                if (slope>=0)
                    prevy=j-1;
                else prevy=j+1;
            }
            //File ouptut = new File("lines.png");
            //ImageIO.write(test2, "png", ouptut);
        }
        catch(Exception e) {}
    }
    void findColors() {
        color=0;
        for (int k=0;k<4;k++)
        {
            for (int i=0;i<width;i+=interval)
            {
                for (int j=0;j<height;j+=interval)
                {
                    counter=0;
                    int rg=image.getPixel(i, j);
                   // Log.d("coordinates","("+i+","+j+")");
                    if (colorTest(rg,k,color_detection))
                    {
                        //System.out.println("phase");
                        //Color newColor2 = new Color(colorcode[color][0],colorcode[color][1],colorcode[color][2]);
                        switch (color) {
                            case 0:
                            test.setPixel(i, j, Color.BLUE);
                            test2.setPixel(i, j, Color.BLUE);
                            break;
                            case 1:
                                test.setPixel(i, j, Color.RED);
                                test2.setPixel(i, j, Color.RED);
                            break;
                            case 2:
                                test.setPixel(i, j, Color.GREEN);
                                test2.setPixel(i, j, Color.GREEN);
                            break;
                            case 3:
                                test.setPixel(i, j, Color.YELLOW);
                                test2.setPixel(i, j, Color.YELLOW);
                            //System.out.println(counter);
                        }

                    }
                }
            }
            color++;
        }

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private class sendDataASync extends AsyncTask<String, Void, String> {
        @Override
        protected String doInBackground(String... message) {

            // params comes from the execute() call: params[0] is the url.
            EditText ip=(EditText)findViewById(R.id.server_ip);
            EditText portstring=(EditText)findViewById(R.id.server_port);
            String serverName = ip.getText().toString();
            int port = Integer.parseInt(portstring.getText().toString());
            try {
                System.out.println("Connecting to " + serverName
                        + " on port " + port);
                Socket client = new Socket(serverName, port);
                System.out.println("Just connected to "
                        + client.getRemoteSocketAddress());
                OutputStream outToServer = client.getOutputStream();
                DataOutputStream out =
                        new DataOutputStream(outToServer);

                out.writeUTF(messagefinal);

                InputStream inFromServer = client.getInputStream();
                DataInputStream in =
                        new DataInputStream(inFromServer);
                //System.out.println("Server says " + in.readUTF());
                client.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return "abc";
        }


        // onPostExecute displays the results of the AsyncTask.
        @Override
        protected void onPostExecute(String result) {

        }
    }
}


