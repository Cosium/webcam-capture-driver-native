import javax.swing.*;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferInt;
import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;
import java.nio.file.Files;
import java.nio.file.Paths;

public class WebcamCaptureTestShowCapturedFile {
	public static void main(String[] args) throws IOException {
		byte[] data = Files.readAllBytes(Paths.get((new File(args[0])).getAbsolutePath()));
		ByteBuffer byteBufferARGB = ByteBuffer.wrap(data);
		byteBufferARGB.order(ByteOrder.nativeOrder());
		IntBuffer argbImageInt = byteBufferARGB.asIntBuffer();
		final int[] pixels = new int[argbImageInt.capacity()];
		argbImageInt.get(pixels);
		BufferedImage image = new BufferedImage( 640, 480, BufferedImage.TYPE_INT_ARGB );
		final int[] a = ( (DataBufferInt) image.getRaster().getDataBuffer() ).getData();
		System.arraycopy(pixels, 0, a, 0, pixels.length);

		JFrame frame = new JFrame();
		frame.getContentPane().add(new JLabel(new ImageIcon(image)));
		frame.pack();
		frame.setVisible(true);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
}
