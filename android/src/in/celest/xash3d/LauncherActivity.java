package in.celest.xash3d;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.content.Intent;
import android.widget.EditText;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.Button;
import android.widget.TextView;
import android.content.ComponentName;
import android.content.pm.PackageManager;
import android.content.SharedPreferences;
import android.app.Dialog;
import android.os.Build;

import in.celest.xash3d.clientmod.R;

public class LauncherActivity extends Activity {
  public final static int sdk = Integer.valueOf(Build.VERSION.SDK);
	static EditText cmdArgs;
	static SharedPreferences mPref;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		if ( sdk >= 21 )
			super.setTheme( 0x01030224 );
		else super.setTheme( 0x01030005 );
        // Build layout
	setContentView(R.layout.activity_launcher);
        cmdArgs = (EditText)findViewById(R.id.cmdArgs);
		cmdArgs.setSingleLine(true);
		mPref = getSharedPreferences("mod", 0);
		cmdArgs.setText(mPref.getString("argv","-console"));
		
		ExtractAssets.extractPAK(this, false);
	}
	
	public void aboutXash(View view)
	{
		final Activity a = this;
		this.runOnUiThread(new Runnable() 
		{
			public void run()
			{
				final Dialog dialog = new Dialog(a);
				dialog.setContentView(R.layout.about);
				dialog.setCancelable(true);
				dialog.show();
			}
		});
	}
	
	public void startXash(View view)
	{
		String argv = cmdArgs.getText().toString();
		Intent intent = new Intent();
		intent.setAction("in.celest.xash3d.START");
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);

		SharedPreferences.Editor editor = mPref.edit();
		editor.putString("argv", argv);
		editor.commit();

		if(cmdArgs.length() != 0) intent.putExtra("argv", argv);
		// Uncomment to set gamedir here
		//intent.putExtra("gamedir", "valve" );
		intent.putExtra("gamelibdir", getFilesDir().getAbsolutePath().replace("/files","/lib"));
		intent.putExtra("pakfile", getFilesDir().getAbsolutePath() + "/extras.pak");

		PackageManager pm = getPackageManager();
		if(intent.resolveActivity(pm) != null)
		{
			startActivity(intent);
		}
		else
		{
			showXashInstallDialog("Xash3D FWGS ");
		}
	}
	
	public void showXashInstallDialog(String msg)
	{
		AlertDialog.Builder builder = new AlertDialog.Builder(this);

		builder.setTitle("Xash error")
		.setMessage(msg + getString(R.string.alert_dialog_text))
		.show();
	}
}
