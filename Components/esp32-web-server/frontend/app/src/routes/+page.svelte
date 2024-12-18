<script>
  const ESP32_IP = "192.168.238.49";
  
  async function analogRead(pin) {
    const res = await fetch(`http://${ESP32_IP}/analogRead/${pin}`);
    const text = await res.text();
    return text;
  };
</script>

<main class="p-4 w-full grid bg-slate-900">
  {#await analogRead(32)}
    caricamento...
  {:then value} 
  <input type="range" min={0} max={2**12} {value} disabled />
  <p class="text-white text-3xl">{value}</p>
  {/await}
</main>

<style>
  :global(body, main, html) {
    @apply h-full;
  }
</style>

