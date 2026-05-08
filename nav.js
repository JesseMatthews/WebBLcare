/*
  nav.js — Bloomhaven Care
  ─────────────────────────────────────────────────────────────
  This file injects the navigation bar and footer into EVERY
  page automatically. Edit the nav links or footer here ONCE
  and all pages update instantly.

  HOW IT WORKS:
  Each HTML page has:
    <div id="nav-placeholder"></div>   ← nav goes here
    <div id="footer-placeholder"></div> ← footer goes here
  This script fills those divs when the page loads.

  ACTIVE LINK HIGHLIGHTING:
  Each page sets:  const CURRENT_PAGE = 'services';
  This script then adds class="active" to the matching nav link.
*/

/* ═══════════════════════════════════════════════
   NAV HTML — edit links here to update all pages
   ═══════════════════════════════════════════════ */
const NAV_HTML = `
<nav id="mainNav">

  <!-- Logo — links back to home page -->
  <a class="nav-logo" href="index.html" title="Bloomhaven Care — Home">
    <img src="logo_transparent.png" alt="Bloomhaven Care Logo">
  </a>

  <!-- Desktop navigation links -->
  <!-- To add a page: add a link here AND create the .html file -->
  <div class="nav-links">
    <a href="index.html"    data-page="home">Home</a>
    <a href="services.html" data-page="services">Services</a>
    <a href="about.html"    data-page="about">About Us</a>
    <a href="team.html"     data-page="team">Our Team</a>
    <a href="recruit.html"  data-page="recruit">Join Us</a>
    <a href="gallery.html"  data-page="gallery">Gallery</a>
    <a href="faq.html"      data-page="faq">FAQ</a>
    <!-- Blue CTA button -->
    <a href="contact.html"  data-page="contact" class="nav-cta">Contact Us</a>
  </div>

  <!-- Hamburger — mobile only -->
  <button class="hamburger" id="hamburger" onclick="toggleMenu()" aria-label="Open menu">
    <span></span><span></span><span></span>
  </button>

</nav>

<!-- Mobile dropdown menu -->
<div class="mobile-menu" id="mobileMenu">
  <a href="index.html">🏠 Home</a>
  <a href="services.html">✨ Services</a>
  <a href="about.html">💛 About Us</a>
  <a href="team.html">👥 Our Team</a>
  <a href="recruit.html">🌟 Join Us</a>
  <a href="gallery.html">📸 Gallery</a>
  <a href="faq.html">❓ FAQ</a>
  <a href="contact.html">📬 Contact Us</a>
</div>
`;

/* ═══════════════════════════════════════════════
   FOOTER HTML — edit once, updates all pages
   CHANGE: Update phone, email, address, social links
   ═══════════════════════════════════════════════ */
const FOOTER_HTML = `
<footer>
  <div class="container">
    <div class="footer-grid">

      <!-- Brand column -->
      <div>
        <img src="logo_transparent.png" alt="Bloomhaven Care" class="footer-logo-img">
        <p class="footer-tagline">Compassionate Care. Trusted Support. Better Life.</p>
        <!-- CHANGE: Update company description -->
        <p class="footer-desc">Professional, compassionate home care services delivered with dignity, respect, and genuine kindness. Trusted by hundreds of families.</p>
        <!-- CHANGE: Update social media URLs -->
        <div class="footer-socials">
          <a class="social-btn" href="https://facebook.com"  target="_blank" rel="noopener" title="Facebook">f</a>
          <a class="social-btn" href="https://instagram.com" target="_blank" rel="noopener" title="Instagram">📷</a>
          <a class="social-btn" href="https://twitter.com"   target="_blank" rel="noopener" title="Twitter">𝕏</a>
          <a class="social-btn" href="https://linkedin.com"  target="_blank" rel="noopener" title="LinkedIn">in</a>
        </div>
      </div>

      <!-- Services column -->
      <div class="footer-col">
        <h4>Services</h4>
        <ul class="footer-links">
          <li><a href="services.html">Personal Care</a></li>
          <li><a href="services.html">Medication Support</a></li>
          <li><a href="services.html">Companionship</a></li>
          <li><a href="services.html">Meal Preparation</a></li>
          <li><a href="services.html">Live-In Care</a></li>
          <li><a href="services.html">Dementia Care</a></li>
        </ul>
      </div>

      <!-- Company column -->
      <div class="footer-col">
        <h4>Company</h4>
        <ul class="footer-links">
          <li><a href="about.html">About Us</a></li>
          <li><a href="team.html">Our Team</a></li>
          <li><a href="gallery.html">Gallery</a></li>
          <li><a href="recruit.html">Careers</a></li>
          <li><a href="faq.html">FAQ</a></li>
          <li><a href="contact.html">Contact</a></li>
        </ul>
      </div>

      <!-- Contact column -->
      <!-- CHANGE: Update these contact details -->
      <div class="footer-col">
        <h4>Contact</h4>
        <ul class="footer-links">
          <li><a href="tel:input">📞 07436125564</a></li>
          <li><a href="email input here">✉️ info@bloomhavencare.co.uk</a></li>
          <li><a>📍 Adress input here</a></li>
          <li><a>🕐 Mon–Fri 8am–6pm</a></li>
        </ul>
      </div>

    </div><!-- end footer-grid -->

    <!-- Bottom bar -->
    <div class="footer-bottom">
      <!-- CHANGE: Update company name / year -->
      <p>© 2026 Bloomhaven Care Ltd. All rights reserved.</p>
      <div class="footer-certs">
        <span class="cert-badge">CQC Regulated</span>
        <span class="cert-badge">ICO Registered</span>
        <span class="cert-badge">Investors in People</span>
      </div>
      <div style="display:flex;gap:18px;">
        <a href="#">Privacy Policy</a>
        <a href="#">Terms of Service</a>
        <a href="#">Cookies</a>
      </div>
    </div>

  </div>
</footer>
`;

/* ═══════════════════════════════════════════════
   INJECT NAV + FOOTER INTO PAGE
   ═══════════════════════════════════════════════ */
document.addEventListener('DOMContentLoaded', () => {

  /* Inject nav */
  const navSlot = document.getElementById('nav-placeholder');
  if (navSlot) navSlot.innerHTML = NAV_HTML;

  /* Inject footer */
  const footerSlot = document.getElementById('footer-placeholder');
  if (footerSlot) footerSlot.innerHTML = FOOTER_HTML;

  /* Highlight the active nav link based on CURRENT_PAGE variable
     Each page sets:  const CURRENT_PAGE = 'about';  etc. */
  if (typeof CURRENT_PAGE !== 'undefined') {
    /* Desktop nav */
    document.querySelectorAll('.nav-links a[data-page]').forEach(link => {
      if (link.getAttribute('data-page') === CURRENT_PAGE) {
        link.classList.add('active');
      }
    });
    /* Mobile nav */
    document.querySelectorAll('.mobile-menu a').forEach(link => {
      const href = link.getAttribute('href') || '';
      if (href.includes(CURRENT_PAGE)) {
        link.classList.add('active');
      }
    });
  }

  /* Scroll shadow on nav */
  window.addEventListener('scroll', () => {
    const nav = document.getElementById('mainNav');
    if (nav) nav.classList.toggle('scrolled', window.scrollY > 10);
  });

});

/* Mobile menu toggle */
function toggleMenu() {
  const menu   = document.getElementById('mobileMenu');
  const burger = document.getElementById('hamburger');
  if (!menu || !burger) return;
  menu.classList.toggle('open');
  burger.classList.toggle('open');
}

/* Close mobile menu when clicking outside */
document.addEventListener('click', e => {
  const menu   = document.getElementById('mobileMenu');
  const burger = document.getElementById('hamburger');
  if (menu && menu.classList.contains('open')) {
    if (!menu.contains(e.target) && burger && !burger.contains(e.target)) {
      menu.classList.remove('open');
      burger.classList.remove('open');
    }
  }
});