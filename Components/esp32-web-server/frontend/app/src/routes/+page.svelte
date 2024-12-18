<script>
  const ESP32_IP = "192.168.238.49";

  async function analogRead(pin) {
    const controller = new AbortController();
    const signal = controller.signal;
    try {
      const res = await fetch(`http://${ESP32_IP}/analogRead/${pin}`, {
        signal,
      });
      const text = await res.text();
      return Number(text);
    } catch {
      controller.abort();
    }
  }

  async function analogWrite(pin, value) {
    const controller = new AbortController();
    const signal = controller.signal;
    try {
      const res = await fetch(`http://${ESP32_IP}/analogWrite/${pin}/${value}`);
    } catch {
      controller.abort();
    }
  }

  function map(value, inMin, inMax, outMin, outMax) {
    return Math.round(
      ((value - inMin) * (outMax - outMin)) / (inMax - inMin) + outMin,
    );
  }

  let potenziometroValue = 0;

  setInterval(async () => {
    try {
      potenziometroValue = await analogRead(32);
      await analogWrite(2, map(potenziometroValue, 0, 2 ** 12, 0, 255));
    } catch (err) {
      console.error(err);
    }
  }, 350);
</script>

<main class="p-4 w-full grid bg-slate-900">
  <input
    type="range"
    min={0}
    max={2 ** 12}
    value={potenziometroValue}
    disabled
  />
  <p class="text-white text-3xl">{potenziometroValue}</p>
</main>

<style>
  :global(body, main, html) {
    @apply h-full;
  }
</style>
