// ServitorConnectHasher.js
// converted from the interactive implementation at https://reden777.github.io/servitorconnect/2SCNewHash.html

const WORKER_CODE = `
    self.onmessage = function(e) {
        const { baseIntent, repeats, iterations, batchId } = e.data;
        let lastResult = null;

        for (let r = 0; r < repeats; r++) {
            let intentionValue = baseIntent;

            for (let i = 0; i < iterations; i++) {
                let h = 0;

                // First iteration: handle raw Uint8Array bytes
                if (i === 0 && intentionValue instanceof Uint8Array) {
                    for (let j = 0; j < intentionValue.length; j++) {
                        h = (h * 31 + intentionValue[j]) >>> 0;
                    }
                } else {
                    let strVal = String(intentionValue);
                    for (let j = 0; j < strVal.length; j++) {
                        h = (h * 31 + strVal.charCodeAt(j)) >>> 0;
                    }
                }

                intentionValue = h;
            }
            lastResult = intentionValue;
        }

        // Return the final hash of the batch
        self.postMessage({ batchId, lastResult });
    };
`;

export class IntentHasher {
    /**
     * @param {Object} options
     * @param {string | Uint8Array} options.intent - Text string or binary Uint8Array
     * @param {number} options.iterations - Number of hash iterations per repeat
     * @param {number} options.repeats - Number of repeats per cycle
     * @param {number} options.duration - Total run duration in seconds
     * @param {number} [options.interval=3600] - Interval between batches in seconds (default: 1 hour)
     * @param {Function} [options.onTick] - Called every second: (remainingSeconds) => {}
     * @param {Function} [options.onBatchComplete] - Called when a batch finishes: ({ batchId, lastResult }) => {}
     * @param {Function} [options.onComplete] - Called when the duration finishes
     * @param {Function} [options.onStop] - Called if manually stopped
     */
    constructor(options) {
        this.intent = options.intent;
        this.iterations = options.iterations;
        this.repeats = options.repeats;
        this.duration = options.duration;
        this.interval = options.interval || 3600;

        // Callbacks
        this.onTick = options.onTick || (() => {});
        this.onBatchComplete = options.onBatchComplete || (() => {});
        this.onComplete = options.onComplete || (() => {});
        this.onStop = options.onStop || (() => {});

        this.worker = null;
        this.timerInterval = null;
        this.isRunning = false;
        this.batchCount = 0;

        // Create object URL for Worker
        const blob = new Blob([WORKER_CODE], { type: 'application/javascript' });
        this.workerUrl = URL.createObjectURL(blob);
    }

    /**
     * Helper to load a Browser File/Blob object into a Uint8Array
     */
    static async fileToUint8Array(file) {
        const arrayBuffer = await file.arrayBuffer();
        return new Uint8Array(arrayBuffer);
    }

    /**
     * Formats seconds into HH:MM:SS
     */
    static formatTime(totalSeconds) {
        const hrs = Math.floor(totalSeconds / 3600).toString().padStart(2, '0');
        const mins = Math.floor((totalSeconds % 3600) / 60).toString().padStart(2, '0');
        const secs = (totalSeconds % 60).toString().padStart(2, '0');
        return `${hrs}:${mins}:${secs}`;
    }

    _sendBatch() {
        this.batchCount++;
        this.worker.postMessage({
            baseIntent: this.intent,
            repeats: this.repeats,
            iterations: this.iterations,
            batchId: this.batchCount
        });
    }

    start() {
        if (this.isRunning) return;

        if (!this.intent) throw new Error("Intent cannot be empty.");
        if (this.iterations <= 0 || this.repeats <= 0 || this.duration <= 0) {
            throw new Error("iterations, repeats, and duration must be positive numbers.");
        }

        this.isRunning = true;
        this.worker = new Worker(this.workerUrl);

        this.worker.onmessage = (e) => {
            this.onBatchComplete(e.data);
        };

        const startTime = Math.floor(Date.now() / 1000);
        const endTime = startTime + this.duration;
        let nextBatchTime = startTime + this.interval;

        // Dispatch initial batch
        this._sendBatch();
        this.onTick(this.duration);

        this.timerInterval = setInterval(() => {
            const currentTime = Math.floor(Date.now() / 1000);

            if (currentTime >= endTime) {
                this.stop(true);
                return;
            }

            if (currentTime >= nextBatchTime) {
                this._sendBatch();
                nextBatchTime += this.interval;
            }

            const remaining = Math.max(0, endTime - currentTime);
            this.onTick(remaining);
        }, 1000);
    }

    stop(completed = false) {
        if (!this.isRunning) return;

        clearInterval(this.timerInterval);
        if (this.worker) {
            this.worker.terminate();
            this.worker = null;
        }

        this.isRunning = false;

        if (completed) {
            this.onComplete();
        } else {
            this.onStop();
        }
    }

    destroy() {
        this.stop();
        URL.revokeObjectURL(this.workerUrl);
    }
}
