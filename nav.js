/*
  nav.js — Bloomhaven Care (Warm Futurism Rebuild)
  ──────────────────────────────────────────────────
  Injects the sticky header and footer into every page.
  The header keeps the brand fixed on the left with navigation to the right.
*/

/* ══════════════════════════════════════════════
   HEADER HTML — Fixed logo, right aligned nav
   ══════════════════════════════════════════════ */
const HEADER_HTML = `
<div id="scrollProgress"></div>

<header id="siteHeader" class="at-top">
  <div class="header-inner">

    <!-- Brand -->
    <a class="nav-brand" href="index.html" aria-label="Bloomhaven Care home">
      <span class="brand-mark" aria-hidden="true">
        <img src="logo_symbol_transparent.png" alt="">
      </span>
      <span class="brand-copy">
        <span class="brand-title">Bloomhaven Care</span>
        <span class="brand-subtitle">Homecare Agency</span>
      </span>
    </a>

    <!-- Navigation -->
    <nav class="nav-right" aria-label="Primary navigation">
      <a href="index.html"    data-page="home">Home</a>
      <a href="services.html" data-page="services">Services</a>
      <a href="about.html"    data-page="about">About Us</a>
      <a href="recruit.html"  data-page="recruit">Join Us</a>
      <a href="faq.html"      data-page="faq">FAQ</a>
      <a href="contact.html"  data-page="contact" class="nav-cta">Contact Us</a>
    </nav>

    <!-- Hamburger (mobile) -->
    <button class="hamburger" id="hamburger" aria-label="Toggle navigation menu" aria-expanded="false" aria-controls="mobileMenu">
      <span></span><span></span><span></span>
    </button>

  </div>
</header>

<!-- Mobile full-screen menu -->
<div id="mobileMenu" role="dialog" aria-label="Navigation menu" aria-hidden="true">
  <div class="mobile-menu-brand" aria-label="Bloomhaven Care homecare agency">
    <span class="brand-mark" aria-hidden="true">
      <img src="logo_symbol_transparent.png" alt="">
    </span>
    <span class="brand-copy">
      <span class="brand-title">Bloomhaven Care</span>
      <span class="brand-subtitle">Homecare Agency</span>
    </span>
  </div>
  <a href="index.html"    data-page="home">Home</a>
  <a href="services.html" data-page="services">Services</a>
  <a href="about.html"    data-page="about">About Us</a>
  <a href="recruit.html"  data-page="recruit">Join Us</a>
  <a href="faq.html"      data-page="faq">FAQ</a>
  <a href="contact.html"  class="mobile-cta">Contact Us →</a>
</div>
`;

/* ══════════════════════════════════════════════
   FOOTER HTML
   ══════════════════════════════════════════════ */
const FOOTER_HTML = `
<footer>
  <div class="container">
    <div class="footer-grid">

      <!-- Brand -->
      <div>
        <div class="footer-logo-wrap">
          <img src="logo_transparent.png" alt="Bloomhaven Care" class="footer-logo-img">
        </div>
        <p class="footer-tagline">Compassionate Care. Trusted Support. Better Life.</p>
        <p class="footer-desc">Professional homecare agency supporting people across Preston and nearby communities with dignified, practical care at home.</p>
      </div>

      <!-- Services -->
      <div class="footer-col">
        <h4>Our Services</h4>
        <ul class="footer-links">
          <li><a href="services.html">Personal Care</a></li>
          <li><a href="services.html">Medication Support</a></li>
          <li><a href="services.html">Companionship</a></li>
          <li><a href="services.html">Meal Preparation</a></li>
          <li><a href="services.html">Live-In Care</a></li>
          <li><a href="services.html">Dementia Care</a></li>
          <li><a href="services.html">Night Care</a></li>
        </ul>
      </div>

      <!-- Company -->
      <div class="footer-col">
        <h4>Company</h4>
        <ul class="footer-links">
          <li><a href="about.html">About Us</a></li>
          <li><a href="recruit.html">Careers</a></li>
          <li><a href="faq.html">FAQ</a></li>
          <li><a href="contact.html">Contact Us</a></li>
        </ul>
      </div>

      <!-- Contact -->
      <div class="footer-col">
        <h4>Get In Touch</h4>
        <ul class="footer-links">
          <li><a href="tel:07436125564">Phone: 07436 125564</a></li>
          <li><a class="footer-email" href="mailto:info@bloomhavencare.co.uk">Email: info@bloomhavencare.co.uk</a></li>
          <li><span aria-label="Service area">Area: Lancashire(Preston, Lytham, Fleetwood ans Blackpool)</span></li>
          <li><a href="https://www.google.com/maps/search/?api=1&query=4%20Derby%20Street%2C%20Preston%2C%20PR1%201DT" target="_blank" rel="noopener" aria-label="Registered office address">Office: 4 Derby Street, Preston, PR1 1DT</a></li>
          <li><span aria-label="Opening hours">Hours: Mon–Fri 8am–6pm</span></li>
          <li><span aria-label="Saturday hours">Saturday: 9am–1pm</span></li>
        </ul>
      </div>

    </div><!-- /footer-grid -->

    <!-- Bottom bar -->
    <div class="footer-bottom">
      <p>Copyright 2026 Bloomhaven Care Ltd. All rights reserved. Registered in England & Wales. Company No. 17172482.</p>
      <div class="footer-certs">
        <!-- CQC badge paused for later:
        <a class="cert-badge" href="https://www.cqc.org.uk/provider/REPLACE_WITH_CQC_PROVIDER_ID" target="_blank" rel="noopener">CQC Registered</a>
        -->
        <span class="cert-badge">ICO Registered</span>
        <span class="cert-badge">DBS Checked Staff</span>
      </div>
      <div class="footer-bottom-links">
        <a href="privacy.html">Privacy Policy</a>
        <a href="terms.html">Terms</a>
        <a href="cookies.html">Cookies</a>
      </div>
    </div>

  </div>
</footer>

<!-- Floating WhatsApp button -->
<a class="whatsapp-btn" href="https://wa.me/447436125564?text=Hello%2C%20I%27d%20like%20to%20enquire%20about%20home%20care." target="_blank" rel="noopener" aria-label="Chat on WhatsApp">
  <svg xmlns="http://www.w3.org/2000/svg" width="26" height="26" viewBox="0 0 24 24" fill="currentColor"><path d="M17.472 14.382c-.297-.149-1.758-.867-2.03-.967-.273-.099-.471-.148-.67.15-.197.297-.767.966-.94 1.164-.173.199-.347.223-.644.075-.297-.15-1.255-.463-2.39-1.475-.883-.788-1.48-1.761-1.653-2.059-.173-.297-.018-.458.13-.606.134-.133.298-.347.446-.52.149-.174.198-.298.298-.497.099-.198.05-.371-.025-.52-.075-.149-.669-1.612-.916-2.207-.242-.579-.487-.5-.669-.51-.173-.008-.371-.01-.57-.01-.198 0-.52.074-.792.372-.272.297-1.04 1.016-1.04 2.479 0 1.462 1.065 2.875 1.213 3.074.149.198 2.096 3.2 5.077 4.487.709.306 1.262.489 1.694.625.712.227 1.36.195 1.871.118.571-.085 1.758-.719 2.006-1.413.248-.694.248-1.289.173-1.413-.074-.124-.272-.198-.57-.347m-5.421 7.403h-.004a9.87 9.87 0 01-5.031-1.378l-.361-.214-3.741.982.998-3.648-.235-.374a9.86 9.86 0 01-1.51-5.26c.001-5.45 4.436-9.884 9.888-9.884 2.64 0 5.122 1.03 6.988 2.898a9.825 9.825 0 012.893 6.994c-.003 5.45-4.437 9.884-9.885 9.884m8.413-18.297A11.815 11.815 0 0012.05 0C5.495 0 .16 5.335.157 11.892c0 2.096.547 4.142 1.588 5.945L.057 24l6.305-1.654a11.882 11.882 0 005.683 1.448h.005c6.554 0 11.89-5.335 11.893-11.893a11.821 11.821 0 00-3.48-8.413z"/></svg>
  <span class="whatsapp-label">WhatsApp</span>
</a>

<!-- Floating call button (mobile only) -->
<a class="float-call" href="tel:07436125564" aria-label="Call us now">
  <svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"><path d="M22 16.92v3a2 2 0 01-2.18 2 19.79 19.79 0 01-8.63-3.07A19.5 19.5 0 013.07 10.8a19.79 19.79 0 01-3.07-8.68A2 2 0 012 0h3a2 2 0 012 1.72 12.84 12.84 0 00.7 2.81 2 2 0 01-.45 2.11L6.09 7.91a16 16 0 006 6l1.27-1.27a2 2 0 012.11-.45 12.84 12.84 0 002.81.7A2 2 0 0122 14.92z"/></svg>
  Call Us Now
</a>
`;

const HOURS_NOTICE_HTML = `<div id="hoursNotice" class="hours-notice" role="status" aria-live="polite" hidden></div>`;
const OFFICE_HOURS = [
  null,
  { open: 8, close: 18 },
  { open: 8, close: 18 },
  { open: 8, close: 18 },
  { open: 8, close: 18 },
  { open: 8, close: 18 },
  { open: 9, close: 13 }
];

const DAY_NAMES = ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'];

function getLondonTimeParts(date = new Date()) {
  const parts = new Intl.DateTimeFormat('en-GB', {
    timeZone: 'Europe/London',
    weekday: 'short',
    hour: '2-digit',
    minute: '2-digit',
    hour12: false
  }).formatToParts(date);
  const get = (type) => parts.find(p => p.type === type).value;
  const dayMap = { Sun: 0, Mon: 1, Tue: 2, Wed: 3, Thu: 4, Fri: 5, Sat: 6 };
  return {
    day: dayMap[get('weekday')],
    minutes: parseInt(get('hour'), 10) * 60 + parseInt(get('minute'), 10)
  };
}

function isOfficeOpen(date = new Date()) {
  const { day, minutes } = getLondonTimeParts(date);
  const schedule = OFFICE_HOURS[day];
  if (!schedule) return false;
  return minutes >= schedule.open * 60 && minutes < schedule.close * 60;
}

function formatOpenTime(hour) {
  return hour === 9 ? '9am' : `${hour}am`;
}

function getClosedNotice(date = new Date()) {
  const { day, minutes } = getLondonTimeParts(date);
  const schedule = OFFICE_HOURS[day];
  const urgent = 'For urgent existing client needs, call 07436 125564.';

  if (schedule && minutes < schedule.open * 60) {
    return `We're currently closed — we'll respond to your message when we open at ${formatOpenTime(schedule.open)} today. ${urgent}`;
  }

  for (let offset = 1; offset <= 7; offset++) {
    const nextDay = (day + offset) % 7;
    if (OFFICE_HOURS[nextDay]) {
      return `We're currently closed — we'll respond to your message on ${DAY_NAMES[nextDay]}. ${urgent}`;
    }
  }

  return `We're currently closed — we'll respond to your message soon. ${urgent}`;
}

function updateHoursNotice() {
  const hoursNotice = document.getElementById('hoursNotice');
  if (!hoursNotice) return;

  if (isOfficeOpen()) {
    hoursNotice.hidden = true;
    document.body.classList.remove('hours-notice-visible');
  } else {
    hoursNotice.textContent = getClosedNotice();
    hoursNotice.hidden = false;
    document.body.classList.add('hours-notice-visible');
  }
}

/* ══════════════════════════════════════════════
   INJECT INTO PAGE
   ══════════════════════════════════════════════ */
document.addEventListener('DOMContentLoaded', () => {

  /* Inject header */
  const headerSlot = document.getElementById('header-placeholder');
  if (headerSlot) {
    headerSlot.innerHTML = HEADER_HTML;
    headerSlot.insertAdjacentHTML('afterend', HOURS_NOTICE_HTML);
    updateHoursNotice();
  }

  /* Inject footer */
  const footerSlot = document.getElementById('footer-placeholder');
  if (footerSlot) footerSlot.innerHTML = FOOTER_HTML;

  const injectedFooter = document.querySelector('footer');
  if (injectedFooter && 'IntersectionObserver' in window) {
    const footerObserver = new IntersectionObserver(entries => {
      document.body.classList.toggle('footer-in-view', entries.some(entry => entry.isIntersecting));
    }, { threshold: 0.08 });
    footerObserver.observe(injectedFooter);
  }

  /* ── Active nav links ── */
  if (typeof CURRENT_PAGE !== 'undefined') {
    document.querySelectorAll('[data-page]').forEach(link => {
      if (link.getAttribute('data-page') === CURRENT_PAGE) {
        link.classList.add('active');
      }
    });
  }

  /* ── Scroll behaviour: header collapse + progress bar ── */
  const header = document.getElementById('siteHeader');
  const progress = document.getElementById('scrollProgress');

  function onScroll() {
    const scrolled = window.scrollY;
    const maxScroll = document.body.scrollHeight - window.innerHeight;

    /* Header state */
    if (header) {
      if (scrolled > 60) {
        header.classList.remove('at-top');
        header.classList.add('scrolled');
        document.body.classList.add('header-scrolled');
      } else {
        header.classList.add('at-top');
        header.classList.remove('scrolled');
        document.body.classList.remove('header-scrolled');
      }
    }

    /* Scroll progress bar */
    if (progress && maxScroll > 0) {
      progress.style.width = ((scrolled / maxScroll) * 100) + '%';
    }
  }

  window.addEventListener('scroll', onScroll, { passive: true });
  onScroll(); // run on load

  /* ── Mobile menu toggle ── */
  const hamburger = document.getElementById('hamburger');
  const mobileMenu = document.getElementById('mobileMenu');

  function openMenu() {
    mobileMenu.classList.add('open');
    hamburger.classList.add('open');
    hamburger.setAttribute('aria-expanded', 'true');
    mobileMenu.setAttribute('aria-hidden', 'false');
    document.body.style.overflow = 'hidden';
  }

  function closeMenu() {
    mobileMenu.classList.remove('open');
    hamburger.classList.remove('open');
    hamburger.setAttribute('aria-expanded', 'false');
    mobileMenu.setAttribute('aria-hidden', 'true');
    document.body.style.overflow = '';
  }

  if (hamburger && mobileMenu) {
    hamburger.addEventListener('click', () => {
      mobileMenu.classList.contains('open') ? closeMenu() : openMenu();
    });

    /* Close on link click */
    mobileMenu.querySelectorAll('a').forEach(a => {
      a.addEventListener('click', closeMenu);
    });

    /* Close on outside click */
    document.addEventListener('click', e => {
      if (mobileMenu.classList.contains('open') &&
          !mobileMenu.contains(e.target) &&
          !hamburger.contains(e.target)) {
        closeMenu();
      }
    });

    /* Escape key */
    document.addEventListener('keydown', e => {
      if (e.key === 'Escape' && mobileMenu.classList.contains('open')) closeMenu();
    });
  }

  /* ── Scroll reveal (IntersectionObserver) ── */
  const revealEls = document.querySelectorAll('.reveal');
  if (revealEls.length) {
    const revealObserver = new IntersectionObserver((entries) => {
      entries.forEach((entry, i) => {
        if (entry.isIntersecting) {
          setTimeout(() => entry.target.classList.add('visible'), i * 80);
          revealObserver.unobserve(entry.target);
        }
      });
    }, { threshold: 0.12 });
    revealEls.forEach(el => revealObserver.observe(el));
  }

});
