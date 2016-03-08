import com.github.sarxos.webcam.Webcam;
import com.github.sarxos.webcam.WebcamPanel;
import com.github.sarxos.webcam.WebcamResolution;
import com.github.sarxos.webcam.ds.nativeapi.NativeWebcamDriver;

import javax.swing.*;
import java.awt.*;
import java.util.List;

public class WebcamCaptureTestUseNativeDriver {
	public static void main(String[] args) throws InterruptedException {

		Webcam.setDriver(new NativeWebcamDriver());

		List<Webcam> webcams = Webcam.getWebcams();
		System.out.println("Number of Webcams found: "+webcams.size());
		for (Webcam wc: webcams) {
			System.out.println(wc);
			System.out.println("Supported Resolutions: ");
			for(Dimension res: wc.getViewSizes()) {
				System.out.println(res);
			}
		}

		Webcam webcam = Webcam.getDefault();
		if (webcam != null) {
			webcam.setViewSize(WebcamResolution.VGA.getSize());

			WebcamPanel panel = new WebcamPanel(webcam);
			panel.setFPSDisplayed(true);
			panel.setDisplayDebugInfo(true);
			panel.setImageSizeDisplayed(true);

			JFrame window = new JFrame("Test webcam panel");
			window.add(panel);
			window.setResizable(true);
			window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
			window.pack();
			window.setVisible(true);
		}
	}
}
