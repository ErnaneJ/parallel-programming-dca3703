import numpy as np
import plotly.graph_objects as go
import os

N = 32
SNAPSHOT_INTERVAL = 10
NSTEPS = 1000
BINARY_FILE = './task-11.impact-of-schedule-and-collapse-clauses/data/fluid_with_perturbation.bin'

def read_snapshots(filename: str) -> tuple[np.ndarray, int]:
    """
    Reads 3D float32 array snapshots from a binary file.

    Parameters:
        filename (str): Path to the binary file.

    Returns:
        tuple: (data, num_snapshots)
            - data: numpy array of shape (num_snapshots, N, N, N)
            - num_snapshots: total number of snapshots in the file
    """
    size_bytes = os.path.getsize(filename)
    snapshot_size = N * N * N * 4
    num_snapshots = size_bytes // snapshot_size
    print(f"Detected {num_snapshots} snapshots in '{filename}'.")

    with open(filename, 'rb') as f:
        data = np.fromfile(f, dtype=np.float32).reshape((num_snapshots, N, N, N))
    return data, num_snapshots

def plot_with_slider(snapshots: np.ndarray, num_snapshots: int):
    """
    Displays a 3D slider animation of the fluid simulation's Z-slices.

    Parameters:
        snapshots (np.ndarray): Array of shape (num_snapshots, N, N, N).
        num_snapshots (int): Number of time snapshots.
    """
    mid_slice = N // 2
    frames = []

    for i, snapshot in enumerate(snapshots):
        z_data = snapshot[:, :, mid_slice]
        frames.append(go.Frame(data=[go.Surface(z=z_data, colorscale="Viridis")], name=str(i)))

    initial_z = snapshots[0][:, :, mid_slice]
    fig = go.Figure(
        data=[go.Surface(z=initial_z, colorscale="Viridis")],
        frames=frames
    )

    fig.update_layout(
        title="Fluid Perturbation Evolution",
        scene=dict(
            xaxis_title='X',
            yaxis_title='Y',
            zaxis_title='Velocity',
            camera_eye=dict(x=1.5, y=1.5, z=0.5)
        ),
        width=800,
        height=700,
        updatemenus=[dict(
            type="buttons",
            buttons=[dict(
                label="Play",
                method="animate",
                args=[None, {"frame": {"duration": 100, "redraw": True}, "fromcurrent": True}]
            )],
            showactive=False
        )],
        sliders=[dict(
            steps=[dict(
                method="animate",
                args=[[str(k)], {
                    "mode": "immediate",
                    "frame": {"duration": 100, "redraw": True},
                    "transition": {"duration": 0}
                }],
                label=f"Step {k * SNAPSHOT_INTERVAL}"
            ) for k in range(num_snapshots)],
            transition={"duration": 0},
            x=0,
            y=0,
            currentvalue={"font": {"size": 16}, "prefix": "Snapshot: ", "visible": True, "xanchor": "center"},
            len=1.0
        )]
    )

    fig.show()

if __name__ == '__main__':
    snapshots, num_snapshots = read_snapshots(BINARY_FILE)
    plot_with_slider(snapshots, num_snapshots)
