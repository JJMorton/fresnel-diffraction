import numpy as np
import matplotlib.pyplot as plt
from matplotlib import colors

def plot_intensity(arr):
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    X = np.arange(arr.shape[0])
    Y = np.arange(arr.shape[1])[::-1]
    plt.pcolormesh(X, Y, (np.abs(arr)**2), cmap='gray', shading="nearest")
    plt.colorbar()
    ax.set_aspect(1)
    ax.set_xticks([])
    ax.set_yticks([])
    return fig
    
def plot_phase_amp(arr):
    def colorise(z, cmap_name: str):
        cmap = plt.get_cmap(cmap_name)
        norm = colors.Normalize(vmin=0, vmax=1)
        r = norm(np.abs(z))
        arg = norm(np.angle(z))

        h = colors.rgb_to_hsv(cmap(arg)[:, :, :3])[:, :, 0]
        v = r
        hv_array = np.stack((h, v), axis=2)

        c = np.apply_along_axis(lambda c: colors.hsv_to_rgb((c[0], 1.0, c[1])), axis=2, arr=hv_array)
        return c

    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    plt.imshow(colorise(arr.T, "jet"))
    plt.xticks([])
    plt.yticks([])
    ax.set_aspect(1)
    return fig

if __name__ == "__main__":
	aperture = np.genfromtxt("out/aperture.csv", delimiter=',', dtype=complex)
	fig = plot_intensity(aperture)
	fig.suptitle("Aperture")
	plt.savefig("out/aperture.png")

	screen = np.genfromtxt("out/screen.csv", delimiter=',', dtype=complex)
	fig = plot_intensity(screen)
	fig.suptitle("Screen")
	plt.savefig("out/screen.png")
